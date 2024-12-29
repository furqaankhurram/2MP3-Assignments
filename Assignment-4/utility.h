#ifndef UTILITY_H
#define UTILITY_H


#define THRESHOLD 1e-15


//structs and typedefs
typedef double (*ObjectiveFunction)(int, double *);

typedef struct Bound{
    double lowerBound;
    double upperBound;
}Bound;

typedef struct Particle {
	
	double *velocity;
	double *position;
	double best_value;
	double *best_position;
	double value;

} Particle;

// Function prototypes
double random_double(double min, double max);
double pso(ObjectiveFunction objective_function,const char *func_name, int NUM_VARIABLES, Bound *bounds, int NUM_PARTICLES, int MAX_ITERATIONS, double best_position[], double w, double c1, double c2);
void make_particles(Particle *particles, int NUM_PARTICLES, int NUM_VARIABLES, Bound *bounds, ObjectiveFunction objective_function);
void update(Particle *particles, int NUM_PARTICLES, int NUM_VARIABLES, double *best_position, double w, double c1, double c2, ObjectiveFunction objective_function, Bound *bounds);

#endif // UTILITY_H
