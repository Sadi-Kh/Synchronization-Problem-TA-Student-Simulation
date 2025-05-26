# Compiler
CC = gcc

# Flags
CFLAGS = -Wall -pthread

# Output binary name
TARGET = ta_simulation

# Source files
SRCS = ta_simulation.c

# Default target
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

# Clean up
clean:
	rm -f $(TARGET)
