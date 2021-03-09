.PHONY: all tests

RELEASE_OPTIONS = -O2 -std=c++17

all:
	g++ main.cpp pattern_matching.cpp -o main.out -g -lOpenCL $(RELEASE_OPTIONS)

tests:
	g++ tests/stress.cpp pattern_matching.cpp -o stress.out -lOpenCL $(RELEASE_OPTIONS)
	g++ tests/time.cpp pattern_matching.cpp -o time.out -lOpenCL $(RELEASE_OPTIONS)