# Compiler
CC = gcc

# Compiler flags
CFLAGS = -pthread -Wall

# Target executable
TARGET = memory_test

# Source files
SRCS = memory_test.c

# Default rule
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Clean rule to remove the executable
clean:
	rm -f $(TARGET)

# Phony targets (do not treat as files)
.PHONY: all clean
