.PHONY: all tests debug

RELEASE_OPTIONS = -O2 -std=c++17
DEBUG_OPTIONS = -g -std=c++17

all:
	g++ main.cpp pattern_matching/pattern_matching.cpp -o main.out -lOpenCL $(RELEASE_OPTIONS)

debug:
	g++ main.cpp pattern_matching/pattern_matching.cpp -o main_debug.out -lOpenCL $(DEBUG_OPTIONS)

tests:
	g++ tests/stress.cpp pattern_matching/pattern_matching.cpp -o stress.out -lOpenCL $(RELEASE_OPTIONS)
	g++ tests/time.cpp pattern_matching/pattern_matching.cpp -o time.out -lOpenCL $(RELEASE_OPTIONS)