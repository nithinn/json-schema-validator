
# Where to find user code.
JVAL_DIR    = ..
JSON_DIR    = $(JVAL_DIR)/jsoncpp
JSON_INC    = $(JSON_DIR)/json
JVAL_SRC    = $(JVAL_DIR)/src
SRC_DIR		= .

# Flags passed to the preprocessor.
CPPFLAGS += -I$(JVAL_SRC)/ -I$(JSON_INC)/

# Flags passed to the C++ compiler.
CXXFLAGS += --std=c++0x -Wall -Wextra -pthread

# Flags passed to the C++ linker
LDFLAGS = -lm

SAMPLE = sample 

OBJS = validator.o primitive.o keyword_validator.o jsoncpp.o

all : $(SAMPLE)

clean :
	rm -f $(SAMPLE) *.o *.a

jvalidator.a : $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

# Builds a sample test.
validator.o : $(JVAL_SRC)/validator.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(JVAL_SRC)/validator.cpp

keyword_validator.o : $(JVAL_SRC)/keyword_validator.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(JVAL_SRC)/keyword_validator.cpp

primitive.o : $(JVAL_SRC)/primitive.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(JVAL_SRC)/primitive.cpp

jsoncpp.o : $(JSON_DIR)/jsoncpp.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(JSON_DIR)/jsoncpp.cpp

sample.o : $(SRC_DIR)/sample.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SRC_DIR)/sample.cpp

sample : sample.o jvalidator.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $^ -o $@
