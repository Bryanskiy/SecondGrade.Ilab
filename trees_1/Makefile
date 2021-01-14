.PHONY: all debug

all:
	g++ main.cpp -o main.out -O2
	g++ testing/random_test.cpp -o random_test.out -O2

debug:
	g++ main.cpp -D "DEBUG_" -o main.out -g -O2
	g++ testing/random_test.cpp -D "DEBUG_" -o random_test.out -g -O2