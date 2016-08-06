# Unit Testing Makefile
# IMPORTANT : Use GMOCK 1.7.0 is NOT compatible with this build. 

GTEST_DIR = $(HOME)/utils/gtest-1.7.0

# Where to find user code.
JVAL_DIR    = ../..
JSON_DIR    = $(JVAL_DIR)/jsoncpp
JSON_INC    = $(JSON_DIR)/json
JVAL_SRC    = $(JVAL_DIR)/src
JVAL_UTDIR  = .

# Flags passed to the preprocessor.
# Set Google Test's header directory as a system directory, such that
# the compiler doesn't generate warnings in Google Test headers.
CPPFLAGS += -I$(GTEST_DIR)/include/ -I$(GTEST_DIR)/include/gtest/ 
CPPFLAGS += -I$(GTEST_DIR)/include/gtest/internal/ -I$(JVAL_SRC) 
CPPFLAGS += -I../ -I$(JVAL_DIR)/jsoncpp/json

# Flags passed to the C++ compiler.
CXXFLAGS += --std=c++0x -g3 -Wall -Wextra -pthread

LDFLAGS = -lm

# All tests produced by this Makefile.  Remember to add new tests you
# created to the list.
TESTS = jvalut 

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

# House-keeping build targets.

all : $(TESTS)

clean :
	rm -f $(TESTS) gtest.a gtest_main.a *.o $(OBJS)

# Builds gtest.a and gtest_main.a.

# Usually you shouldn't tweak such internal variables, indicated by a
# trailing _.
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

OBJS = \
	primitive_ut.o \
	jval_ut.o \
	validator.o \
	primitive.o \
	keyword_validator.o \
	jsoncpp.o

# For simplicity and to avoid depending on Google Test's
# implementation details, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Test
# compiles fast and for ordinary users its source rarely changes.
gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
		$(GTEST_DIR)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

# Builds a sample test.
validator.o : $(JVAL_SRC)/validator.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(JVAL_SRC)/validator.cpp

keyword_validator.o : $(JVAL_SRC)/keyword_validator.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(JVAL_SRC)/keyword_validator.cpp

primitive.o : $(JVAL_SRC)/primitive.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(JVAL_SRC)/primitive.cpp

jsoncpp.o : $(JSON_DIR)/jsoncpp.cpp $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(JSON_DIR)/jsoncpp.cpp

jval_ut.o : $(JVAL_UTDIR)/jval_ut.cpp $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(JVAL_UTDIR)/jval_ut.cpp

primitive_ut.o : $(JVAL_UTDIR)/primitive_ut.cpp $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(JVAL_UTDIR)/primitive_ut.cpp

jvalut : $(OBJS) gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $^ -o $@
