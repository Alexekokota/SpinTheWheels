# Project settings
PROJECT_NAME = game

# Define default compiler
CC = g++

# Define the Raylib installation path and include path
RAYLIB_INSTALL_PATH = /usr/local/lib
RAYLIB_INCLUDE_PATH = /usr/local/include

# Platform detection and setup
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
    PLATFORM_OS = LINUX
    LDFLAGS += -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
    CFLAGS += -I$(RAYLIB_INCLUDE_PATH)
endif

# Define all source files required
SRCS := $(wildcard src/*.cpp)
OBJS := $(SRCS:.cpp=.o)

# Define compilation flags
CFLAGS += -Wall -std=c++11

# Compilation and linking
all: $(PROJECT_NAME)

$(PROJECT_NAME): $(OBJS)
	@echo "Linking..."
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.cpp
	@echo "Compiling $<..."
	$(CC) -c $< -o $@ $(CFLAGS)

# Clean up object files and binary
clean:
	@echo "Cleaning up..."
	rm -f $(OBJS) $(PROJECT_NAME)

