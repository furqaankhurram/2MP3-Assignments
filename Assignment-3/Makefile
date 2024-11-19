
# Compiler
CC = gcc

# Compiler flags
CFLAGS = -g -Wall -Wno-unused-variable

# Target executable name
TARGET = sentiment_analyzer

# Source files
SRCS = main.c vaderSentiment.c 

# Object files
OBJS = $(SRCS:.c=.o)

# Default rule to build the target
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(CFLAGS) -lm

# Rule to compile each .c file to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule to remove compiled files
clean:
	rm -f $(TARGET) $(OBJS)

# Valgrind rule to check for memory leaks
valgrind: $(TARGET)
	valgrind --leak-check=full --track-origins=yes ./$(TARGET)

