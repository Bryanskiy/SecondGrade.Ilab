all: main unit_tests determinant_swap_tests

main:
	g++ main.cpp -o main.out

unit_tests:
	g++ testing/unit_tests/main.cpp -o unit_tests.out -lgtest -lpthread

determinant_swap_tests:
	g++ testing/determinant_swap_tests/random.cpp -o random.out
