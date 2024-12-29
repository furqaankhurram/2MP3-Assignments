# Compiler
CC = gcc

# Compiler Flags
CFLAGS = -Wall -Wextra -O2 -Ofast -march=native -mtune=native -w
LDFLAGS = -lm  # Link the math library

# Source files
SRCS = main.c PSO.c OF.c

# Object files (derived from SRCS)
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = pso

# Build the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Compile each source file into an object file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Run the program using valgrind
valgrind: $(TARGET)
	valgrind --leak-check=full --track-origins=yes ./$(TARGET) $(ARGS)

# Run the program with optimization flags for fast execution
runfast: $(TARGET)
	./$(TARGET) $(ARGS)

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean valgrind runfast

