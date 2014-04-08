#Makefile for cpp_proj eda031
# Define the compiler and the linker. The linker must be defined since
# the implicit rule for linking uses CC as the linker. g++ can be
# changed to clang++.
CXX = g++
CC  = g++

# Configuracion de C++
CPPFLAGS =  -std=c++11
CXXFLAGS =  -O2 -Wall -Wextra -pedantic-errors -Wold-style-cast 
CXXFLAGS += -std=c++11 
CXXFLAGS += -g
LDFLAGS =   -g

# Variables
#TARGET programet
PROGS1 = zerversql
PROG2 = client
DIR_SRC  = ../src/
DIR_HELP = ../help/
DIR_OBJ  = ../obj/
DIR_BIN  = ../bin/

PROGS = zerver client zerversql

all: $(PROGS)

zerver: zerver.o connection.o newsgroup.o server.o frontend.o backend.o
client: alpha_client.o connection.o
zerversql: zerversql.o connection.o  server.o frontend.o backendsql.o

# Phony targets
.PHONY: all clean

# Standard clean
clean:
	rm -f *.o $(DIR_OBJ)/$(PROGS)
