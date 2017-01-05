###############################################################################
# Filename: Makefile
# Description: Automatic build instructions for xzes40/transformer
# Thanks: http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
###############################################################################

CPP_C       = clang++
CPP_FLAGS   = -Wall -g

BUILD_DIR   = build
LIBRARY_DIR = lib
INCLUDE_DIR = include
SRC_DIR     = src

LIBRARIES = -Iinclude -lxalan-c -lxerces-c -lstdc++

_SRC = cache.cpp lib.cpp transform.cpp main.cpp
SRC  = $(patsubst %,$(SRC_DIR)/%,$(_SRC))

_DEPS = cache.hpp lib.hpp transform.hpp
DEPS  = $(patsubst %,$(INCLUDE_DIR)/%,$(_DEPS))

_OBJS = cache.o lib.o transform.o main.o
OBJS  = $(patsubst %,$(BUILD_DIR)/%,$(_OBJS))

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CPP_C) -c -o $@ $< $(CFLAGS) $(LIBRARIES)

main: $(OBJS)
	$(CPP_C) -o $(BUILD_DIR)/$@ $^ $(CPP_FLAGS) $(LIBRARIES)

.DEFAULT_GOAL := main

.PHONY: clean
clean:
	rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/main
