#!/bin/bash

g++ ./main.cpp  -o main.out
g++ ./testing/stress_tests/algorithm.cpp -o stress_tests.out
g++ ./testing/geometry_unit_tests/main.cpp -lgtest -lpthread -o unit_tests.out
