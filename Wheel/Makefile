.PHONY: all clean

# Define required raylib variables
PROJECT_NAME       ?= game
RAYLIB_PATH        ?= ../..

# Define default options
PLATFORM           ?= PLATFORM_RPI

# Define default C++ compiler: g++
CC = g++

# Define source and object directories
SRC_DIR = src
OBJ_DIR = obj

# Find all .cpp source files in the SRC_DIR
SRC = $(wildcard $(SRC_DIR)/*.cpp)

# Define object files from source files
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

# Define include paths for required headers
INCLUDE_PATHS = -I. -I$(RAYLIB_PATH)/src -I$(RAYLIB_PATH)/src/external

# Define library paths containing required libs.
LDFLAGS = -L$(RAYLIB_PATH)/src -L$(RAYLIB_PATH)/release/libs -L/opt/vc/lib

# Define any libraries required on linking
LDLIBS = -lraylib -lbrcmGLESv2 -lbrcmEGL -lbcm_host -lpthread -lrt -lm -ldl

# Default target entry
all: $(OBJ_DIR) $(PROJECT_NAME)

# Create object directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Project target defined by PROJECT_NAME
$(PROJECT_NAME): $(OBJS)
	$(CC) -o $(PROJECT_NAME) $(OBJS) $(CFLAGS) $(INCLUDE_PATHS) $(LDFLAGS) $(LDLIBS)

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDE_PATHS)

# Clean everything
clean:
	rm -rf $(OBJ_DIR) $(PROJECT_NAME)
	@echo Cleaning done
