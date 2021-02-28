.PHONY: all debug tests

RELEASE_OPTIONS = -O2 -std=c++17
DEBUG_OPTIONS = -g -std=c++17 -D"DEBUG" -fno-elide-constructors
GTEST_OPTIONS = -lgtest -lpthread

all: 
	$(MAKE) -C parser
	g++ main.cpp circuit/circuit.cpp parser/lex.yy.cc parser/parser.tab.cc -o main.out $(RELEASE_OPTIONS)

debug: 
	$(MAKE) -C parser
	g++ main.cpp circuit/circuit.cpp parser/lex.yy.cc parser/parser.tab.cc -o main_debug.out $(DEBUG_OPTIONS)

tests:
	g++ tests/matrix/main.cpp -o matrix_tests.out $(RELEASE_OPTIONS) $(GTEST_OPTIONS)
	g++ tests/circuit/circuit_random_test.cpp circuit/circuit.cpp -o circuit_tests.out $(RELEASE_OPTIONS)




