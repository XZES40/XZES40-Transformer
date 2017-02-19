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
CPP_FLAGS   = -Wall -g -v -std=c++11

BUILD_DIR   = build
LIBRARY_DIR = lib
INCLUDE_DIR = include
SRC_DIR     = src

LIBRARIES = -I./include -lxalan-c -lxerces-c

_SRC = cache.cpp lib.cpp document.cpp transform.cpp
SRC  = $(patsubst %,$(SRC_DIR)/%,$(_SRC))

_DEPS = cache.hpp lib.hpp document.hpp transform.hpp
DEPS  = $(patsubst %,$(INCLUDE_DIR)/%,$(_DEPS))

_OBJS = cache.o lib.o document.o transform.o
OBJS  = $(patsubst %,$(BUILD_DIR)/%,$(_OBJS))

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CPP_C) -c -o $@ $< $(CFLAGS) $(LIBRARIES)

main: $(OBJS)
	$(CPP_C) -o $(BUILD_DIR)/main src/main.cpp $(OBJS) $(CPP_FLAGS) $(LIBRARIES)

daemon: $(OBJS)
	$(CPP_C) -o $(BUILD_DIR)/daemon src/daemon.cpp $(OBJS) $(CPP_FLAGS) $(LIBRARIES)

main.py: $(SRC_DIR)/main.py
	if [ ! -f `pwd`/$(BUILD_DIR)/main.py ]; \
		then ln -s `pwd`/$(SRC_DIR)/main.py `pwd`/$(BUILD_DIR)/main.py; \
	fi

all: main daemon main.py

.DEFAULT_GOAL := all

.PHONY: clean
clean:
	rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/main $(BUILD_DIR)/daemon $(BUILD_DIR)/main.py
