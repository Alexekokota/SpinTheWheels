#**************************************************************************************************
#
#   raylib makefile for Desktop platforms, Raspberry Pi, Android and HTML5
#
#   Copyright (c) 2013-2019 Ramon Santamaria (@raysan5)
#
#   This software is provided "as-is", without any express or implied warranty. In no event
#   will the authors be held liable for any damages arising from the use of this software.
#
#   Permission is granted to anyone to use this software for any purpose, including commercial
#   applications, and to alter it and redistribute it freely, subject to the following restrictions:
#
#     1. The origin of this software must not be misrepresented; you must not claim that you
#     wrote the original software. If you use this software in a product, an acknowledgment
#     in the product documentation would be appreciated but is not required.
#
#     2. Altered source versions must be plainly marked as such, and must not be misrepresented
#     as being the original software.
#
#     3. This notice may not be removed or altered from any source distribution.
#
#**************************************************************************************************

.PHONY: all clean

# Define required raylib variables
PROJECT_NAME       ?= game
RAYLIB_VERSION     ?= 5.0.0
RAYLIB_PATH        ?= ../..

# Define compiler path on Windows
COMPILER_PATH      ?= C:/raylib/w64devkit/bin

# Define default options
# One of PLATFORM_DESKTOP, PLATFORM_RPI, PLATFORM_ANDROID, PLATFORM_WEB
PLATFORM           ?= PLATFORM_DESKTOP

# Locations of your newly installed library and associated headers. See ../src/Makefile
# On Linux, if you have installed raylib but cannot compile the examples, check that
# the *_INSTALL_PATH values here are the same as those in src/Makefile or point to known locations.
# To enable system-wide compile-time and runtime linking to libraylib.so, run ../src/$ sudo make install RAYLIB_LIBTYPE_SHARED.
# To enable compile-time linking to a special version of libraylib.so, change these variables here.
# To enable runtime linking to a special version of libraylib.so, see EXAMPLE_RUNTIME_PATH below.
# If there is a libraylib in both EXAMPLE_RUNTIME_PATH and RAYLIB_INSTALL_PATH, at runtime,
# the library at EXAMPLE_RUNTIME_PATH, if present, will take precedence over the one at RAYLIB_INSTALL_PATH.
# RAYLIB_INSTALL_PATH should be the desired full path to libraylib. No relative paths.
DESTDIR ?= /usr/local
RAYLIB_INSTALL_PATH ?= $(DESTDIR)/lib
# RAYLIB_H_INSTALL_PATH locates the installed raylib header and associated source files.
RAYLIB_H_INSTALL_PATH ?= $(DESTDIR)/include

# Library type used for raylib: STATIC (.a) or SHARED (.so/.dll)
RAYLIB_LIBTYPE        ?= STATIC

# Build mode for project: DEBUG or RELEASE
BUILD_MODE            ?= RELEASE

# Use external GLFW library instead of rglfw module
# TODO: Review usage on Linux. Target version of choice. Switch on -lglfw or -lglfw3
USE_EXTERNAL_GLFW     ?= FALSE

# Use Wayland display server protocol on Linux desktop
# by default it uses X11 windowing system
USE_WAYLAND_DISPLAY   ?= FALSE

# Determine PLATFORM_OS in case PLATFORM_DESKTOP selected
ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    # No uname.exe on MinGW!, but OS=Windows_NT on Windows!
    # ifeq ($(UNAME),Msys) -> Windows
    ifeq ($(OS),Windows_NT)
        PLATFORM_OS=WINDOWS
        export PATH := $(COMPILER_PATH):$(PATH)
    else
        UNAMEOS=$(shell uname)
        ifeq ($(UNAMEOS),Linux)
            PLATFORM_OS=LINUX
        endif
        ifeq ($(UNAMEOS),FreeBSD)
            PLATFORM_OS=BSD
        endif
        ifeq ($(UNAMEOS),OpenBSD)
            PLATFORM_OS=BSD
        endif
        ifeq ($(UNAMEOS),NetBSD)
            PLATFORM_OS=BSD
        endif
        ifeq ($(UNAMEOS),DragonFly)
            PLATFORM_OS=BSD
        endif
        ifeq ($(UNAMEOS),Darwin)
            PLATFORM_OS=OSX
        endif
    endif
endif
ifeq ($(PLATFORM),PLATFORM_RPI)
    UNAMEOS=$(shell uname)
    ifeq ($(UNAMEOS),Linux)
        PLATFORM_OS=LINUX
    endif
endif

# RAYLIB_PATH adjustment for different platforms.
# If using GNU make, we can get the full path to the top of the tree. Windows? BSD?
# Required for ldconfig or other tools that do not perform path expansion.
ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),LINUX)
        RAYLIB_PREFIX ?= ..
        RAYLIB_PATH    = $(realpath $(RAYLIB_PREFIX))
    endif
endif
# Default path for raylib on Raspberry Pi, if installed in different path, update it!
# This is not currently used by src/Makefile. Not sure of its origin or usage. Refer to wiki.
# TODO: update install: target in src/Makefile for RPI, consider relation to LINUX.
ifeq ($(PLATFORM),PLATFORM_RPI)
    RAYLIB_PATH       ?= /home/pi/raylib
endif

ifeq ($(PLATFORM),PLATFORM_WEB)
    # Emscripten required variables
    EMSDK_PATH          ?= C:/emsdk
    EMSCRIPTEN_VERSION  ?= 1.38.31
    CLANG_VERSION       = e$(EMSCRIPTEN_VERSION)_64bit
    PYTHON_VERSION      = 2.7.13.1_64bit\python-2.7.13.amd64
    NODE_VERSION        = 8.9.1_64bit
    export PATH         = $(EMSDK_PATH);$(EMSDK_PATH)\clang\$(CLANG_VERSION);$(EMSDK_PATH)\node\$(NODE_VERSION)\bin;$(EMSDK_PATH)\python\$(PYTHON_VERSION);$(EMSDK_PATH)\emscripten\$(EMSCRIPTEN_VERSION);C:\raylib\MinGW\bin:$$(PATH)
    EMSCRIPTEN          = $(EMSDK_PATH)\emscripten\$(EMSCRIPTEN_VERSION)
endif

# Define raylib release directory for compiled library.
# RAYLIB_RELEASE_PATH points to provided binaries or your freshly built version
RAYLIB_RELEASE_PATH 	?= $(RAYLIB_PATH)/src

# EXAMPLE_RUNTIME_PATH embeds a custom runtime location of libraylib.so or other desired libraries
# into each example binary compiled with RAYLIB_LIBTYPE=SHARED. It defaults to RAYLIB_RELEASE_PATH
# so that these examples link at runtime with your version of libraylib.so in ../release/libs/linux
# without formal installation from ../src/Makefile. It aids portability and is useful if you have
# multiple versions of raylib, have raylib installed to a non-standard location, or want to
# bundle libraylib.so with your game. Change it to your liking.
# NOTE: If, at runtime, there is a libraylib.so at both EXAMPLE_RUNTIME_PATH and RAYLIB_INSTALL_PATH,
# The library at EXAMPLE_RUNTIME_PATH, if present, will take precedence over RAYLIB_INSTALL_PATH,
# Implemented for LINUX below with CFLAGS += -Wl,-rpath,$(EXAMPLE_RUNTIME_PATH)
# To see the result, run readelf -d core/core_basic_window; looking at the RPATH or RUNPATH attribute.
# To see which libraries a built example is linking to, ldd core/core_basic_window;
# Look for libraylib.so.1 => $(RAYLIB_INSTALL_PATH)/libraylib.so.1 or similar listing.
EXAMPLE_RUNTIME_PATH   ?= $(RAYLIB_RELEASE_PATH)

# Define default C compiler: gcc
# NOTE: define g++ compiler if using C++
CC = g++

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),OSX)
        # OSX default compiler
        CC = clang++
    endif
    ifeq ($(PLATFORM_OS),BSD)
        # FreeBSD, OpenBSD, NetBSD, DragonFly default compiler
        CC = clang++
    endif
endif
ifeq ($(PLATFORM),PLATFORM_RPI)
    ifeq ($(USE_RPI_CROSS_COMPILER),TRUE)
        # Define RPI cross-compiler
        #CC = armv6j-hardfloat-linux-gnueabi-gcc
        CC = $(RPI_TOOLCHAIN)/bin/arm-linux-gnueabihf-gcc
    endif
endif
ifeq ($(PLATFORM),PLATFORM_WEB)
    # HTML5 emscripten compiler
    # WARNING: To compile to HTML5, code must be redesigned 
    # to use emscripten.h and emscripten_set_main_loop()
    CC = emcc
endif

# Define default make program: Mingw32-make
MAKE = mingw32-make

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),LINUX)
        MAKE = make
    endif
    ifeq ($(PLATFORM_OS),OSX)
        MAKE = make
    endif
    ifeq ($(PLATFORM_OS),BSD)
        MAKE = gmake
    endif
endif
ifeq ($(PLATFORM),PLATFORM_RPI)
    MAKE = make
endif
ifeq ($(PLATFORM),PLATFORM_WEB)
    MAKE = make
endif

# Define default C debug/release flags for compilation
CFLAGS += -Wall -std=c++11

# macOS disable warnings (too verbose)
ifeq ($(PLATFORM_OS),OSX)
    CFLAGS += -Wno-missing-braces -Wno-unused-function
endif

# Add custom include folder
CFLAGS += -I$(RAYLIB_H_INSTALL_PATH)

ifeq ($(BUILD_MODE),DEBUG)
    CFLAGS += -g -D_DEBUG
else
    CFLAGS += -O1
endif

# Define standard compilation flags
ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    LDFLAGS += -L$(RAYLIB_INSTALL_PATH) -L$(RAYLIB_RELEASE_PATH)
    ifeq ($(PLATFORM_OS),WINDOWS)
        LDFLAGS += -lraylib -lopengl32 -lgdi32 -lwinmm -lkernel32 -Wl,--subsystem,windows
        # Required for FPC 3.0.4: -Wl,--allow-multiple-definition
    endif
    ifeq ($(PLATFORM_OS),LINUX)
        ifeq ($(RAYLIB_LIBTYPE),SHARED)
            LDFLAGS += -Wl,-rpath,$(EXAMPLE_RUNTIME_PATH)
        endif
        LDFLAGS += -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
        ifeq ($(USE_WAYLAND_DISPLAY),TRUE)
            LDFLAGS += -lwayland-client -lwayland-cursor -lwayland-egl -lxkbcommon
        endif
        ifeq ($(USE_EXTERNAL_GLFW),TRUE)
            LDFLAGS += -lglfw
        endif
    endif
    ifeq ($(PLATFORM_OS),OSX)
        ifeq ($(RAYLIB_LIBTYPE),SHARED)
            LDFLAGS += -rpath @executable_path
        endif
        LDFLAGS += -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
    endif
    ifeq ($(PLATFORM_OS),BSD)
        ifeq ($(RAYLIB_LIBTYPE),SHARED)
            LDFLAGS += -Wl,-rpath,$(EXAMPLE_RUNTIME_PATH)
        endif
        LDFLAGS += -lraylib -lGL -lm -lpthread -lrt -lX11
        ifeq ($(USE_EXTERNAL_GLFW),TRUE)
            LDFLAGS += -lglfw
        endif
    endif
endif
ifeq ($(PLATFORM),PLATFORM_RPI)
    # required for RPI compilation
    CFLAGS += -DPLATFORM_RPI -D_GNU_SOURCE -I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux
    ifeq ($(USE_RPI_CROSS_COMPILER),TRUE)
        LDFLAGS += -L$(RPI_TOOLCHAIN)/lib
    endif
    LDFLAGS += -L/opt/vc/lib -lraylib -lbrcmEGL -lbrcmGLESv2 -lbcm_host -lvcos -lvchiq_arm -lm -lpthread -lrt
endif
ifeq ($(PLATFORM),PLATFORM_WEB)
    CFLAGS += -Os --no-heap-copy --preload-file resources -s USE_GLFW=3 -s TOTAL_MEMORY=16777216 -s ALLOW_MEMORY_GROWTH=1
    LDFLAGS += -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 --shell-file $(RAYLIB_PATH)/src/shell.html
endif

# Define all source files required
SRCS := $(wildcard src/*.c) $(wildcard src/*.cpp)
OBJS := $(SRCS:.c=.o) $(SRCS:.cpp=.o)

all: $(PROJECT_NAME)

# Compile and link binary
$(PROJECT_NAME): $(OBJS)
	@echo "Compiling..."
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Compile source files
%.o: %.c
	@echo "Compiling $<..."
	$(CC) -c $< -o $@ $(CFLAGS)

%.o: %.cpp
	@echo "Compiling $<..."
	$(CC) -c $< -o $@ $(CFLAGS)

# Clean up object files and binary
clean:
	@echo "Cleaning up..."
	rm -f $(OBJS) $(PROJECT_NAME)
