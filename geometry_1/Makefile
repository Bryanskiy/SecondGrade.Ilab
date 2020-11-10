all: main stress_tests unit_tests

main:
	g++ ./main.cpp  -O2 -o main.out

stress_tests:
	g++ ./testing/stress_tests/algorithm.cpp -O2 -o stress_tests.out

unit_tests:
	g++ ./testing/geometry_unit_tests/main.cpp -lgtest -lpthread -O2 -o unit_tests.out