.PHONY: all debug

RELEASE_OPTIONS = -O2 -std=c++17
DEBUG_OPTIONS = -g -std=c++17 -D"DEBUG" -fno-elide-constructors
GTEST_OPTIONS = -lgtest -lpthread

all: 
	$(MAKE) -C parser
	g++ main.cpp circuit/circuit.cpp parser/lex.yy.cc parser/parser.tab.cc -o main.out $(RELEASE_OPTIONS)

debug: 
	$(MAKE) -C parser
	g++ main.cpp circuit/circuit.cpp parser/lex.yy.cc parser/parser.tab.cc -o main.out $(DEBUG_OPTIONS)
	g++ tests/matrix/determinant_tests/tests.cpp -o matrix_determinant_tests.out $(DEBUG_OPTIONS)
	g++ tests/matrix/gauss_tests/tests.cpp -o matrix_gauss_tests.out $(DEBUG_OPTIONS)
	g++ tests/matrix/unit_tests/main.cpp -o matrix_unit_test.out  $(GTEST_OPTIONS)


