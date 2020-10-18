all: main stress_tests unit_tests

main:
	g++ ./main.cpp  -o2 -o main.out

stress_tests:
	g++ ./testing/stress_tests/algorithm.cpp -o2 -o stress_tests.out

unit_tests:
	g++ ./testing/geometry_unit_tests/main.cpp -lgtest -lpthread -o2 -o unit_tests.out
