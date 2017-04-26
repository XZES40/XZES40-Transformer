##############################################################################
# Filename: Makefile
# Description: Automatic build instructions for xzes/transformer
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
TEST_DIR    = test

LIBRARIES = -I./include -lxalan-c -lxerces-c -lpthread

_SRC =  keylist.cpp cache.cpp lib.cpp document.cpp parse.cpp transform.cpp main.cpp 
SRC  = $(patsubst %,$(SRC_DIR)/%,$(_SRC))

_DEPS = keylist.hpp cache.hpp lib.hpp document.hpp parse.hpp transform.hpp
DEPS  = $(patsubst %,$(INCLUDE_DIR)/%,$(_DEPS))

_OBJS = keylist.o cache.o lib.o document.o parse.o transform.o
OBJS  = $(patsubst %,$(BUILD_DIR)/%,$(_OBJS))


$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CPP_C) -c -o $@ $< $(CFLAGS) $(LIBRARIES)

main: $(OBJS)
	$(CPP_C) -o $(BUILD_DIR)/main   $(SRC_DIR)/main.cpp   $(OBJS) $(CPP_FLAGS) $(LIBRARIES)

daemon: $(OBJS)
	$(CPP_C) -o $(BUILD_DIR)/xzes40d $(SRC_DIR)/daemon.cpp $(OBJS) $(CPP_FLAGS) $(LIBRARIES)

test: $(OBJS)
	$(CPP_C) -o $(BUILD_DIR)/cache  $(TEST_DIR)/cache.cpp $(OBJS) $(CPP_FLAGS) $(LIBRARIES)

main.py: $(SRC_DIR)/main.py
	if [ ! -f `pwd`/$(BUILD_DIR)/main.py ]; \
		then ln -s `pwd`/$(SRC_DIR)/main.py `pwd`/$(BUILD_DIR)/main.py; \
	fi

all: main daemon test main.py

no_test: main daemon main.py

.DEFAULT_GOAL := no_test 

.PHONY: clean
clean:
	rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/main $(BUILD_DIR)/daemon $(BUILD_DIR)/cache $(BUILD_DIR)/main.py
