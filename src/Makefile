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
CPPFLAGS =  -stdlib=libc++
CXXFLAGS += -stdlib=libc++
LDFLAGS += -stdlib=libc++

SOURCES=

SOURCE_PATH=.
OBJECT_PATH=../build
SRCFILES=$(patsubst %.cc,$(SOURCE_PATH)/%.cc,$(SOURCES))
OBJFILES=$(patsubst %.cc,$(OBJECT_PATH)/%.o,$(SOURCES))
PROGFILE=$(OBJECT_PATH)

# Phony targets
.PHONY: all clean

# All
all: $(PROGFILE)

$(PROGFILE): $(OBJFILES)
        $(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@

$(OBJECT_PATH)/%.o: $(SOURCE_PATH)/%.cc
        $(COMPILE.cc)  $(OUTPUT_OPTION) $<

# Standard clean
clean:
        $(RM) $(OBJECT_PATH)/*.o $(PROGFILE)

# Generate dependencies in *.d files
# %.d: %.cc
# 	@set -e; rm -f $@; \
#          $(CPP) -MM $(CPPFLAGS) $< > $@.$$$$; \
#          sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
#          rm -f $@.$$$$

# # Include the *.d files
# SRC = $(wildcard *.cc)
# include $(SRC:.cc=.d)
