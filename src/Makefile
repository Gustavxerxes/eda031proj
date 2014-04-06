#
# Makefile to make the file libclientserver.a, containing
# connection.o and server.o
#
# Define the compiler. g++ can be
# changed to clang++.
CXX = g++
CC  = g++

# Define preprocessor, compiler, and linker flags. Uncomment the # lines
# if you use clang++ and wish to use libc++ instead of libstdc++.
CXXFLAGS =  -g -O2 -Wall -W -pedantic-errors
CXXFLAGS += -Wmissing-braces -Wparentheses -Wold-style-cast 
CXXFLAGS += -std=c++11 
CXXFLAGS += -g
LDFLAGS =   -g
# CPPFLAGS =  -stdlib=libc++
# CXXFLAGS += -stdlib=libc++
# LDFLAGS += -stdlib=libc++

SOURCE_DIR = src
OBJECT_DIR = build
SOURCE_FILES = $(SOURCE_DIR)/%.cc
OBJECT_FILES = $(OBJECT_DIR)/%.o


all: 

$(OBJECT_FILES) : $(OBJECT_DIR)/%.o $(SOURCE_DIR)/%.cc
	$(CXX) $()

# Phony targets
.PHONY: all clean

# Standard clean
clean:
	rm -f *.o libclientserver.a
