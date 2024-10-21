# MemContentionTest
C program to test memory contention

## Explanation:
 - CC = gcc: Defines the compiler to use.
 - CFLAGS = -pthread -Wall: Specifies compiler flags. -pthread is necessary for multithreading, and -Wall enables all warnings.
 - TARGET = memory_test: Sets the name of the output executable.
 - SRCS = memory_test.c: Specifies the source file(s).
 - all: The default rule that builds the program.
 - clean: A rule that removes the compiled executable to clean the directory.
