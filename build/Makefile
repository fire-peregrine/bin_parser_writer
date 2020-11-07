# @file      Makefile
# @brief     Makefile
# @author    fire-peregrine
# @date      2020/10/24
# @copyright Copyright (C) fire-peregrine all rights reserved.
# @license   Released under the MIT License.

### Source/Header file settings

# Project root directory
PROJ_ROOT     = ..

# Binary directory (relative from the current directory)
BIN_DIR       = $(PROJ_ROOT)/bin

# Static library
LIB_STATIC    = $(BIN_DIR)/BinParserWriter.a

# Shared library 
LIB_SHARED    = $(BIN_DIR)/libBinParserWriter.so

# Test program
TEST_PROGRAM  = $(BIN_DIR)/bin_parser_test

# List of source file directories (relative from the current directory)
SRC_DIRS      = $(PROJ_ROOT)/src

# List of main-program-related source file directories (relative from the current directory)
MAIN_DIRS     = $(PROJ_ROOT)/src/main

# List of test-related source file directories (relative from the current directory)
TEST_DIRS     = 

# List of header file directories (relative from the current directory)
SRC_INC_DIRS  = $(PROJ_ROOT)/src/include

# List of main-program-related header file directories (relative from the current directory)
MAIN_INC_DIRS = $(PROJ_ROOT)/src/main/include

# List of test-related header file directories (relative from the current directory)
TEST_INC_DIRS = 

# Object file directory
OBJ_ROOT      = ./obj

# Include common settings
include $(PROJ_ROOT)/Makefile.common

# Doxygen directory
DOXYGEN_DIR   = $(PROJ_ROOT)/doxygen

# Plant UML jar file path.
PLANTUML_JAR  = 


### Don't touch!

# Object file directories
OBJ_DIRS       = $(foreach objdir, $(SRC_DIRS) $(MAIN_DIRS) $(TEST_DIRS), $(objdir:$(PROJ_ROOT)/%=$(OBJ_ROOT)/proj/%))

# Source files
SRCS          = $(foreach srcdir, $(SRC_DIRS), $(wildcard $(srcdir)/*.cpp $(srcdir)/*.cc $(srcdir)/*.c))

# main-program-related source files
MAIN_SRCS     = $(foreach srcdir, $(MAIN_DIRS), $(wildcard $(srcdir)/*.cpp $(srcdir)/*.cc $(srcdir)/*.c))

# test-related source files
TEST_SRCS     = $(foreach srcdir, $(TEST_DIRS), $(wildcard $(srcdir)/*.cpp $(srcdir)/*.cc $(srcdir)/*.c))

# Object files.
OBJS           = $(foreach srcfile, $(filter %.cpp, $(SRCS)), $(srcfile:$(PROJ_ROOT)/%.cpp=$(OBJ_ROOT)/proj/%.o)) \
                 $(foreach srcfile, $(filter %.cc,  $(SRCS)), $(srcfile:$(PROJ_ROOT)/%.cc=$(OBJ_ROOT)/proj/%.o)) \
                 $(foreach srcfile, $(filter %.c,   $(SRCS)), $(srcfile:$(PROJ_ROOT)/%.c=$(OBJ_ROOT)/proj/%.o))

# main-program-related object files.
MAIN_OBJS      = $(foreach srcfile, $(filter %.cpp, $(MAIN_SRCS)), $(srcfile:$(PROJ_ROOT)/%.cpp=$(OBJ_ROOT)/proj/%.o)) \
                 $(foreach srcfile, $(filter %.cc,  $(MAIN_SRCS)), $(srcfile:$(PROJ_ROOT)/%.cc=$(OBJ_ROOT)/proj/%.o)) \
                 $(foreach srcfile, $(filter %.c,   $(MAIN_SRCS)), $(srcfile:$(PROJ_ROOT)/%.c=$(OBJ_ROOT)/proj/%.o))

# Test-related object files.
TEST_OBJS      = $(foreach srcfile, $(filter %.cpp, $(TEST_SRCS)), $(srcfile:$(PROJ_ROOT)/%.cpp=$(OBJ_ROOT)/proj/%.o)) \
                 $(foreach srcfile, $(filter %.cc,  $(TEST_SRCS)), $(srcfile:$(PROJ_ROOT)/%.cc=$(OBJ_ROOT)/proj/%.o)) \
                 $(foreach srcfile, $(filter %.c,   $(TEST_SRCS)), $(srcfile:$(PROJ_ROOT)/%.c=$(OBJ_ROOT)/proj/%.o))


### Compiler Settings

SRC_INCLUDE  = $(addprefix -I, $(SRC_INC_DIRS))
MAIN_INCLUDE = $(addprefix -I, $(MAIN_INC_DIRS))
TEST_INCLUDE = $(addprefix -I, $(TEST_INC_DIRS))

CPPFLAGS = -DUNIT_TEST

AR       = ar

CC       = gcc 
CFLAGS   = -g -Wall -fPIC \
		   -DSOFTWARE_AUTHOR=\"$(SOFTWARE_AUTHOR)\" -DSOFTWARE_NAME=\"$(SOFTWARE_NAME)\" -DSOFTWARE_VERSION=\"$(SOFTWARE_VERSION)\"

CXX      = g++
CXXFLAGS = -g -Wall -fPIC -Wno-c++14-compat -std=c++11 \
		   -DSOFTWARE_AUTHOR=\"$(SOFTWARE_AUTHOR)\" -DSOFTWARE_NAME=\"$(SOFTWARE_NAME)\" -DSOFTWARE_VERSION=\"$(SOFTWARE_VERSION)\"

LDFLAGS  = 
LIBS     = -lm

INCLUDE  = $(SRC_INCLUDE) $(MAIN_INCLUDE) $(TEST_INCLUDE)


### Dependencies

# Note: Special variables in Makefile are as follows.
#       $@ ---> Target.
#       $< ---> First prerequisite.
#       $? ---> Prerequisites newer than the target.
#       $^ ---> All prerequisites.

all: clean init build

build: $(LIB_STATIC) $(LIB_SHARED) $(TEST_PROGRAM)

$(LIB_STATIC): $(OBJS)
	$(AR) rvs $@ $^

$(LIB_SHARED): $(OBJS)
	$(CC) -shared $^ $(LDFLAGS) $(LIBS) -o $@

$(TEST_PROGRAM): $(OBJS) $(MAIN_OBJS)
	$(CC) $^ $(LDFLAGS) $(LIBS) -o $@


$(OBJ_ROOT)/proj/%.o: $(PROJ_ROOT)/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

$(OBJ_ROOT)/proj/%.o: $(PROJ_ROOT)/%.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

$(OBJ_ROOT)/proj/%.o: $(PROJ_ROOT)/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(INCLUDE) -c $< -o $@

.PHONY: init clean dump doxygen

clean:
	rm -rf $(OBJ_ROOT)
	rm -rf $(BIN_DIR)
	rm -rf $(DOXYGEN_DIR)
	rm -f doxygen.log
	-@echo "Info: clean completed."

init:
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_ROOT)
	mkdir -p $(OBJ_ROOT)/proj
	mkdir -p $(OBJ_DIRS)

# Dump internal variables.
dump:
	@echo Dump:
	@echo "    PROJ_ROOT     : " $(PROJ_ROOT)
	@echo "    BIN_DIR       : " $(BIN_DIR)
	@echo "    OBJ_ROOT      : " $(OBJ_ROOT)
	@echo "    TARGET        : " $(TARGET)
	@echo "    TEST_TARGET   : " $(TEST_TARGET)
	@echo "    SRC_DIRS      : " $(SRC_DIRS)
	@echo "    MAIN_DIRS     : " $(MAIN_DIRS)
	@echo "    TEST_DIRS     : " $(TEST_DIRS)
	@echo "    SRC_INC_DIRS  : " $(SRC_INC_DIRS)
	@echo "    MAIN_INC_DIRS : " $(MAIN_INC_DIRS)
	@echo "    TEST_INC_DIRS : " $(TEST_INC_DIRS)
	@echo "    OBJ_DIRS      : " $(OBJ_DIRS)
	@echo "    DOXYGEN_DIR   : " $(DOXYGEN_DIR)
	@echo "    SRCS          : " $(SRCS)
	@echo "    MAIN_SRCS     : " $(MAIN_SRCS)
	@echo "    TEST_SRCS     : " $(TEST_SRCS)
	@echo "    OBJS          : " $(OBJS)
	@echo "    MAIN_OBJS     : " $(MAIN_OBJS)
	@echo "    TEST_OBJS     : " $(TEST_OBJS)
	@echo "                    "
	@echo "    CC            : " $(CC)
	@echo "    CPPFLAGS      : " $(CPPFLAGS)
	@echo "    CXX           : " $(CXX)
	@echo "    CXXFLAGS      : " $(CXXFLAGS)
	@echo "    LDFLAGS       : " $(LDFLAGS)
	@echo "    LIBS          : " $(LIBS)
	@echo "    INCLUDE       : " $(INCLUDE)
	@echo "    TEST_INCLUDE  : " $(TEST_INCLUDE)


# Generate doxygen documents
doxygen:
	-@if [ -f Doxyfile ]; then \
		echo "Info: Doxyfile exists." ; \
	else \
		doxygen -g ; \
		sed -i -e "s|^\(OUTPUT_DIRECTORY *\)=.*$$|\1 = $(DOXYGEN_DIR)|g" \
		       -e "s|^\(CREATE_SUBDIRS *\)=.*$$|\1 = YES|g" \
		       -e "s|^\(ALLOW_UNICODE_NAMES *\)=.*$$|\1 = YES|g" \
		       -e "s|^\(FULL_PATH_NAMES *\)=.*$$|\1 = NO|g" \
		       -e "s|^\(OPTIMIZE_OUTPUT_FOR_C *\)=.*$$|\1 = YES|g" \
		       -e "s|^\(EXTRACT_ALL *\)=.*$$|\1 = YES|g" \
		       -e "s|^\(EXTRACT_PRIVATE *\)=.*$$|\1 = YES|g" \
		       -e "s|^\(EXTRACT_STATIC *\)=.*$$|\1 = YES|g" \
		       -e "s|^\(INPUT *\)=.*$$|\1 = $(SRC_DIRS) $(MAIN_DIRS)|g" \
		       -e "s|^\(RECURSIVE *\)=.*$$|\1 = YES|g" \
		       -e "s|^\(SOURCE_BROWSER *\)=.*$$|\1 = YES|g" \
		       -e "s|^\(INLINE_SOURCES *\)=.*$$|\1 = YES|g" \
		       -e "s|^\(REFERENCED_BY_RELATION *\)=.*$$|\1 = YES|g" \
		       -e "s|^\(GENERATE_LATEX *\)=.*$$|\1 = NO|g" \
		       -e "s|^\(HAVE_DOT *\)=.*$$|\1 = YES|g" \
		       -e "s|^\(CALL_GRAPH *\)=.*$$|\1 = YES|g" \
		       -e "s|^\(PLANTUML_JAR_PATH *\)=.*$$|\1 = $(PLANTUML_JAR)|g" \
		       Doxyfile ; \
	fi
	-@doxygen 2>&1 | tee doxygen.log


