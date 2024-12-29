// CODE: include library(s)
#include "utility.h"
#include "OF_lib.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// Helper function to generate random numbers in a range

double random_double(double min, double max) {
    return min + (max - min) * ((double)rand() / RAND_MAX);
}

// CODE: implement other functions here if necessary

void make_particles(Particle *particles, int NUM_PARTICLES, int NUM_VARIABLES, Bound *bounds, ObjectiveFunction objective_function) {

	for (int i = 0; i < NUM_PARTICLES; i++) {

		// Create particle
		Particle particle;

		// Allocate memory for position, velocity, and best_position
		particle.position = malloc(NUM_VARIABLES * sizeof(double));
		particle.velocity = malloc(NUM_VARIABLES * sizeof(double));
		particle.best_position = malloc(NUM_VARIABLES * sizeof(double));

		// Initialize position, velocity, and best_position
		for (int j = 0; j < NUM_VARIABLES; j++) {
			particle.position[j] = random_double(bounds[j].lowerBound, bounds[j].upperBound);
			particle.velocity[j] = 0.0;
		}

		// Get the value of the particle's position
		particle.value = objective_function(NUM_VARIABLES, particle.position);

		// Set the best value to the particle's current value
		particle.best_value = particle.value;

		// Add particle to the list of particles
		particles[i] = particle;
	}
}


double pso(ObjectiveFunction objective_function, const char *func_name, int NUM_VARIABLES, Bound *bounds, int NUM_PARTICLES, int MAX_ITERATIONS, double *best_position, double w, double c1, double c2) {
    // Allocate memory for particles
    Particle *particles = malloc(NUM_PARTICLES * sizeof(Particle));

    // Initialize particles
    make_particles(particles, NUM_PARTICLES, NUM_VARIABLES, bounds, objective_function);

    // Set initial best_position to the first particle's position
    for (int i = 0; i < NUM_VARIABLES; i++) {
        best_position[i] = particles[0].position[i];
    }

    // Variables for fitness tracking
    double previous_global_best = 1e15; // Large initial value
    int consecutive_count = 0;          // Counter for consecutive iterations
    int required_consecutive_iterations = 20; // Target consecutive count

    // Iterate for max iterations
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        double global_best = objective_function(NUM_VARIABLES, best_position); // Evaluate global fitness 
        double w_max = w+0.2;
        double w_min = w-0.3;
        // Perform particle updates
        for (int p = 0; p < NUM_PARTICLES; p++) {
            // Update the velocity and position of the particle
            for (int j = 0; j < NUM_VARIABLES; j++) {

                // Define max_velocity as a fraction of the search space
                double max_velocity = (bounds[j].upperBound - bounds[j].lowerBound) * 0.2;
                double w_new =w_max - (i/MAX_ITERATIONS)*(w_max-w_min);
                // Update position using velocity
                // Update velocity
                particles[p].velocity[j] = w * particles[p].velocity[j]
                    + c1 * random_double(0, 1) * (particles[p].best_position[j] - particles[p].position[j])
                    + c2 * random_double(0, 1) * (best_position[j] - particles[p].position[j]);

                // Clamp velocity Optimization attempt #1
                if((strcmp(func_name,"rastrigin") == 0)||(strcmp(func_name,"rosenbrock")==0)||(strcmp(func_name,"schwefel")==0)){
                    if (particles[p].velocity[j] > max_velocity) {
                    particles[p].velocity[j] = 2*max_velocity;
                    } else if (particles[p].velocity[j] < -max_velocity) {
                    particles[p].velocity[j] = -max_velocity*2;
                    }
                }
                // Exploration vs Exploitation optimization
               if((strcmp(func_name,"rastrigin") == 0)||(strcmp(func_name,"rosenbrock")==0)||(strcmp(func_name,"schwefel")==0)){
                    double w =w_max - (i/MAX_ITERATIONS)*(w_max-w_min);

                }
                
                // Update the position of the particle
                particles[p].position[j] += particles[p].velocity[j];

                // Clamp to within bounds
                if (particles[p].position[j] < bounds[j].lowerBound) {
                    particles[p].position[j] = bounds[j].lowerBound;
                } else if (particles[p].position[j] > bounds[j].upperBound) {
                    particles[p].position[j] = bounds[j].upperBound;
                }
            }

            // Update the value of the particle and the best value if necessary
            particles[p].value = objective_function(NUM_VARIABLES, particles[p].position);
            if (particles[p].value < particles[p].best_value) {
                particles[p].best_value = particles[p].value;
                for (int k = 0; k < NUM_VARIABLES; k++) {
                    particles[p].best_position[k] = particles[p].position[k];
                }
            }

            // Update the global best value and best position if necessary
            if (particles[p].value < global_best) {
                global_best = particles[p].value;
                for (int k = 0; k < NUM_VARIABLES; k++) {
                    best_position[k] = particles[p].position[k];
                }
            }
        }

        // Check fitness difference threshold for consecutive iterations
        if (fabs(global_best - previous_global_best) < THRESHOLD) {
            consecutive_count++;
            if (consecutive_count >= required_consecutive_iterations) {
                printf("Convergence due to fitness change threshold for %d consecutive iterations at iteration %d.\n",
                       required_consecutive_iterations, i + 1);
                break;
            }
        } else {
            consecutive_count = 0; // Reset counter if condition is not met
        }

        // Update previous global best fitness
        previous_global_best = global_best;

        // Check for overall convergence
         if (fabs(global_best) < THRESHOLD) {
            printf("Convergence reached after %d iterations.\n", i + 1);
            break;
        }  
        

        // Debug output for each iteration
       printf("Iteration %d: Best Fitness = %f\n", i + 1, global_best);
    }

    // Free memory
    for (int i = 0; i < NUM_PARTICLES; i++) {
        free(particles[i].position);
        free(particles[i].velocity);
        free(particles[i].best_position);
    }
    free(particles);

    //printf("Using objective function: %s\n", func_name);
    return objective_function(NUM_VARIABLES, best_position); // Return the best fitness value
}


