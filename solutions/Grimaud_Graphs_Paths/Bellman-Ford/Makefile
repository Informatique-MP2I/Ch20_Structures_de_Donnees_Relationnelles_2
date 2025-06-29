# Makefile for building the Floyd-Warshall project

# Compiler
CC = gcc

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
BIN_DIR = bin
DOCS_DIR = docs
DOXYFILE = Doxyfile

# Output binary
TARGET = bellman_ford

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Compiler flags
CFLAGS = -I$(INCLUDE_DIR) -Wall -Wextra -g 
# Linker flags
LDFLAGS =

# Default target
all: $(BIN_DIR)/$(TARGET)

# Rule to build the final binary
$(BIN_DIR)/$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Rule to compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to generate documentation
$(DOCS_DIR):
	doxygen $(DOXYFILE)

# Rule to clean the build directory and remove the binary
clean:
	rm -rf $(BIN_DIR) $(BUILD_DIR) $(BIN_DIR) $(DOCS_DIR)
	find . -name "*~" -exec rm -f {} \;

.PHONY: all docs clean

