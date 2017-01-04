CCOMPILER=clang
COMPILER_FLAGS=-Wall -g -Iinclude -lxalan-c -lxerces-c -lstdc++

CPPCOMPILER=clang++
CPPCOMPILER_FLAGS=-Wall -g -Iinclude -lxalan-c -lxerces-c -lstdc++

BUILD_DIR=bin
SOURCE_DIR=src

cpp: $(SOURCE_DIR)/xzes40.cpp transform.o cache.o
	$(CPPCOMPILER) -o $(BUILD_DIR)/xzes40.out $(SOURCE_DIR)/xzes40.cpp $(CPPCOMPILER_FLAGS)

#c: $(SOURCE_DIR)/xzes40.c
#	$(CCOMPILER) -o $(BUILD_DIR)/xzes40-c $(SOURCE_DIR)/xzes40.c $(COMPILER_FLAGS)

.default: cpp

clean:
	rm -rf bin/*.{out,o}
