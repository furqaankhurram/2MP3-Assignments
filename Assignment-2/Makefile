# Compiler
CC = gcc

# Compiler flags
CFLAGS = -O3 -fPIC

# Target shared library
TARGET = libmysort.so

# Source files
SRC = mySort.c

# Rule to create the shared library
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -shared -o $@ $^

# Clean rule to remove generated files
clean:
	rm -f $(TARGET)
