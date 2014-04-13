#
# Makefile for cpp_proj eda031
#
CXX = g++
CC  = g++

# C++ configurations
CPPFLAGS =  -std=c++11
CXXFLAGS =  -O2 -Wall -Wfatal-errors -pedantic-errors -Wold-style-cast 
CXXFLAGS += -std=c++11 
CXXFLAGS += -g
LDFLAGS =   -g -L/usr/local/lib -L/usr/local/mysql/lib
# CPPFLAGS =  -stdlib=libc++
# CXXFLAGS += -stdlib=libc++
# LDFLAGS += -stdlib=libc++

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SQL_LIBS = -lmysqlcppconn

OBJS_ZER= server.o connection.o frontend.o backendmem.o newsgroup.o zerver.o messagehandler.o
OBJS_SQL= server.o connection.o frontend.o backendsql.o zerversql.o messagehandler.o
OBJS_CLI= connection.o our_client.o messagehandler.o

all: prep zerver zerver-sql client 

# zerver compile and link
zerver: $(addprefix $(OBJ_DIR)/,$(OBJS_ZER))
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(BIN_DIR)/$@ $?

# zerver-sql compile and link
zerver-sql: $(addprefix $(OBJ_DIR)/,$(OBJS_SQL))
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(BIN_DIR)/$@ $? $(SQL_LIBS) 

# Client compile and link
client: $(addprefix $(OBJ_DIR)/,$(OBJS_CLI))
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(BIN_DIR)/$@ $?

# Phony targets
.PHONY: all clean prep

# Create subdirs
prep:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)

# Standard clean
clean:
	rm -r $(OBJ_DIR) $(BIN_DIR)

# Suffix rule for object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	$(CXX) -c $(CXXFLAGS) -o $@ $?



