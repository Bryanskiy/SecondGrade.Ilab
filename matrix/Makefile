.PHONY: all debug

DEBUG_OPTIONS = -fno-elide-constructors -std=c++17 -D "DEBUG_"
RELEASE_OPTIONS = -O2 -std=c++17
GTEST_OPTIONS = -lgtest -lpthread

all: release.out

release.out:
	g++ main.cpp -o main.out $(RELEASE_OPTIONS)
	g++ testing/unit_tests/main.cpp -o unit_tests.out $(GTEST_OPTIONS) $(RELEASE_OPTIONS)
	g++ testing/determinant_tests/random.cpp -o random.out $(RELEASE_OPTIONS)

debug: debug.out

debug.out:
	g++ main.cpp -o main.out $(DEBUG_OPTIONS)
	g++ testing/unit_tests/main.cpp -o unit_tests.out $(GTEST_OPTIONS) $(DEBUG_OPTIONS)
	g++ testing/determinant_tests/random.cpp -o random.out $(DEBUG_OPTIONS)

