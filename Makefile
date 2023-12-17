# Usage:
# make        # Compile
# make run    # Compile and run
# make clean  # Remove ALL binaries and objects

# Directories
SRC_DIR := src
INC_DIR := $(SRC_DIR)/include
BIN_DIR := bin
OBJ_DIR := build

# EXEC name
TARGET_NAME := HttpServer

# Compiler settings
CC := gcc -std=c99 -m64
CFLAGS := -Wall -Wextra -I$(INC_DIR) -MMD -MP

# Platform specific delete
ifeq ($(OS),Windows_NT)
  RM := del /Q
	LIBFLAGS := -lws2_32
else ifeq ($(shell uname -s),Linux)
	RM := rm -f
	LIBFLAGS := # TODO ADD LIBFLAGS FOR LINUX 
endif

# Define rules
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
DEPS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.d,$(SRCS))

# Target
TARGET := $(BIN_DIR)/$(TARGET_NAME)

# Phony targets
.PHONY: all clean

all: $(TARGET)

# Build rule
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBFLAGS)

# Object file rule
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ 

# Include dependency files
-include $(DEPS)

# Clean rule
clean:
	$(RM) $(BIN_DIR)
	$(RM) $(OBJ_DIR)

# Run target
run: $(TARGET)
	./$(TARGET)