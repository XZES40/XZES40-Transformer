###############################################################################
# Filename: Makefile
# Description: Automatic build instructions for xzes40/transformer
# Builds with:
#   $ make --version
#   GNU Make 4.0
#   Built for x86_64-pc-linux-gnu
# Thanks: http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
###############################################################################

CPP_C       = clang++
CPP_FLAGS   = -Wall -g -std=c++11

BUILD_DIR   = build
LIBRARY_DIR = lib
INCLUDE_DIR = include
SRC_DIR     = src

LIBRARIES = -I./include -lxalan-c -lxerces-c

_SRC = cache.cpp lib.cpp document.cpp transform.cpp main.cpp
SRC  = $(patsubst %,$(SRC_DIR)/%,$(_SRC))

_DEPS = cache.hpp lib.hpp document.hpp transform.hpp
DEPS  = $(patsubst %,$(INCLUDE_DIR)/%,$(_DEPS))

_OBJS = cache.o lib.o document.o transform.o main.o
OBJS  = $(patsubst %,$(BUILD_DIR)/%,$(_OBJS))

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CPP_C) -c -o $@ $< $(CFLAGS) $(LIBRARIES)

main: $(OBJS)
	$(CPP_C) -o $(BUILD_DIR)/$@ $^ $(CPP_FLAGS) $(LIBRARIES)

full: $(SRC)
	$(CPP_C) -o $(BUILD_DIR)/main.1 $^ $(CPP_FLAGS) $(LIBRARIES)

.DEFAULT_GOAL := main

.PHONY: clean
clean:
	rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/main $(BUILD_DIR)/main.1
