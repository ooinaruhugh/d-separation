CFLAGS=-Isrc -std=c17
CXXFLAGS=-Isrc -std=c++20
INCLUDES=src
INTERFACE_HEADERS=src/d_separation.hpp src/graph.hpp

TARGETS=demo01 test_cassiopeia test_diamond

ALGORITHM_MODULE=src/d_separation
# ALGORITHM_MODULE=src/bayes_ball

DEBUG ?= 1
ifeq ($(DEBUG), 1)
	CFLAGS   +=-DDEBUG -Og -g$(DEBUG_FORMAT)
	CXXFLAGS +=-DDEBUG -Og -g$(DEBUG_FORMAT)
else
	CFLAGS	 +=-DNDEBUG
	CXXFLAGS +=-DNDEBUG
endif

all: $(TARGETS)

LINK_COMMAND=$(CXX) -o $@ $^ $(CXXFLAGS)

demo01: plain.o $(ALGORITHM_MODULE).o
	$(LINK_COMMAND)
test_cassiopeia: tests/test_driver.o tests/cassiopeia.o $(ALGORITHM_MODULE).o
	$(LINK_COMMAND)
test_diamond: tests/test_driver.o tests/diamond.o $(ALGORITHM_MODULE).o
	$(LINK_COMMAND)
	
%.o: %.cc $(INTERFACE_HEADERS)
	$(CXX) $(CXXFLAGS) -c -o $@ $< 

clean: 
	$(RM) *.o **/*.o $(TARGETS)

.phony: all clean