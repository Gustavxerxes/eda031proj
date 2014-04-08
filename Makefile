# Makefile for cpp_proj eda031
# Define the compiler and the linker. The linker must be defined since
# the implicit rule for linking uses CC as the linker. g++ can be
# changed to clang++.
CXX = g++
CC  = g++

# C++ configurations
CPPFLAGS =  -std=c++11
CXXFLAGS =  -O2 -Wall -Wextra -pedantic-errors -Wold-style-cast 
CXXFLAGS += -std=c++11 
CXXFLAGS += -g
LDFLAGS =   -g
# CPPFLAGS =  -stdlib=libc++
# CXXFLAGS += -stdlib=libc++
# LDFLAGS += -stdlib=libc++

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

OBJS_ZER= server.o connection.o frontend.o backend.o newsgroup.o zerver.o
OBJS_CLI= connection.o alpha_client.o

all: zerver client

# zerver compile and link
zerver: $(OBJ_DIR)/$(OBJS_ZER)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Client compile and link
client: $(OBJ_DIR)/$(OBJS_CLI)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Phony targets
.PHONY: all clean

# Standard clean
clean:
	rm -f $(OBJ_DIR)/* $(BIN_DIR)/*

# Suffix rule
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	$(CXX) -c $(CXXFLAGS) -o $@ $^
