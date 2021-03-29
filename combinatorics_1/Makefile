.PHONY: all debug tests

RELEASE_OPTIONS = -O2 -std=c++17
DEBUG_OPTIONS = -g -std=c++17 -D"DEBUG" -fno-elide-constructors

all:
	g++ main.cpp -o main.out $(RELEASE_OPTIONS)

debug:
	g++ main_debug.cpp -o main.out $(DEBUG_OPTIONS)

tests:
	g++ tests/random.cpp -o random.out $(RELEASE_OPTIONS)