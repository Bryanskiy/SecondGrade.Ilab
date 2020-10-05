#!/bin/bash

g++ ./testing/unit_tests/main.cpp -lgtest -lpthread -o test.out
g++ ./main.cpp -o main.out