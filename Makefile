# ====================================================================================================
# Usage:
# make        # Compile
# make run    # Compile and run
# make clean  # Remove ALL binaries and objects
# ====================================================================================================

.PHONY: all clean run clean_logs

# ============================================================
# BUILD CONFIG
# ============================================================

# Define system: LINUX, WINDOWS
SYSTEM 				?= WINDOWS

# Define project paths
SOURCE_PATH 	?= src
RELEASE_PATH 	?= bin
OBJ_PATH			?= obj

# Define include paths
INCLUDE_PATHS ?= -I$(SOURCE_PATH)/include

# Define compiler
CC 						:= gcc -std=c99 -m64

# Define compiler flags
CFLAGS 				:= -Wall -Wextra -MMD -MP $(INCLUDE_PATHS)


# ============================================================
# SERVER CONFIG
# ============================================================

# Define server logs file path
LOG_FILE_PATH ?= server_logs.log

# Define server release file name
SERVER_NAME 	?= http_server


# ============================================================
# SYSTEM SPECIFIC CONFIG
# ============================================================

# Define SYSTEM specific commands and libs
RM				:= del /Q
LIB_FLAGS	:= -lws2_32


# ============================================================
# FILES AND OUTPUT
# ============================================================

# Define files
SRCS 		?= $(wildcard $(SOURCE_PATH)/*.c)
OBJS 		?= $(patsubst $(SOURCE_PATH)/%.c,$(OBJ_PATH)/%.o,$(SRCS))
DEPS 		?= $(patsubst $(SOURCE_PATH)/%.c,$(OBJ_PATH)/%.d,$(SRCS))

# Define target
TARGET 	?= $(RELEASE_PATH)/$(SERVER_NAME)


# ============================================================
# RULE LIST
# ============================================================

# Include dependency files
-include $(DEPS)

# Define build rule
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB_FLAGS)

# Define object files rule
$(OBJ_PATH)/%.o: $(SOURCE_PATH)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ 


# Compile and build server
all: $(TARGET)

# Delete all output files (.d .o .exe)
clean:
	$(RM) $(RELEASE_PATH)
	$(RM) $(OBJ_PATH)

# Delete server log file
clean_logs:
	$(RM) $(LOG_FILE_PATH)

# Compile and run server
run: $(TARGET)
	./$(TARGET)