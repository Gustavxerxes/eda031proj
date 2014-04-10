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
LDFLAGS =   -g -L/usr/local/lib -L/usr/local/mysql/lib
# CPPFLAGS =  -stdlib=libc++
# CXXFLAGS += -stdlib=libc++
# LDFLAGS += -stdlib=libc++

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

OBJS_ZER= server.o connection.o frontend.o backend.o newsgroup.o zerver.o
OBJS_SQL= server.o connection.o frontend.o backendsql.o zerver.o
OBJS_CLI= connection.o alpha_client.o

SQL_LIBS = -lmysqlcppconn

all: zerver zerver-sql client

# zerver compile and link
zerver: $(addprefix $(OBJ_DIR)/,$(OBJS_ZER))
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(BIN_DIR)/$@ $?

# zerver-sql compile and link
zerver-sql: 
	$(CXX) $(CXXFLAGS) $(LDFLAGS) @(SQL_LIBS) -o $(BIN_DIR)/$@ $(addprefix $(OBJ_DIR)/,$?)

# Client compile and link
client: $(addprefix $(OBJ_DIR)/,$(OBJS_CLI))
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(BIN_DIR)/$@ $?

# Phony targets
.PHONY: all clean

# Standard clean
clean:
	rm -f $(OBJ_DIR)/* $(BIN_DIR)/*

# Suffix rule for object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	$(CXX) -c $(CXXFLAGS) -o $@ $?
