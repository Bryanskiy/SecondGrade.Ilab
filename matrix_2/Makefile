.PHONY: all

RELEASE_OPTIONS = -O2 -std=c++17

all: release.out

release.out:
	g++ main.cpp -o main.out $(RELEASE_OPTIONS)
	g++ testing/determinant_tests/random.cpp -o random.out $(RELEASE_OPTIONS)


