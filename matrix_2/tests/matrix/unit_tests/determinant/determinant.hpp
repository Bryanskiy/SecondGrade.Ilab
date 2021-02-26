#pragma once

#include <gtest/gtest.h>
#include "../../../../matrix/matrix.hpp"
#include <fstream>

TEST(Matrix, Determinant1) {
    std::ifstream input("tests/matrix/unit_tests/determinant/determinant_cases/1.dat");
    matrix::matrix_t<double> m(1, 1);
    input >> m;
    ASSERT_TRUE(matrix::equal(m.det(), 42.0)) << "1.dat";
}

TEST(Matrix, Determinant2) {
    std::ifstream input("tests/matrix/unit_tests/determinant/determinant_cases/2.dat");
    matrix::matrix_t<double> m(7, 7);
    input >> m;
    ASSERT_TRUE(matrix::equal(m.det(), 126.0)) << "2.dat";
}

TEST(Matrix, Determinant3) {
    std::ifstream input("tests/matrix/unit_tests/determinant/determinant_cases/3.dat");
    matrix::matrix_t<double> m(25, 25);
    input >> m;
    ASSERT_TRUE(matrix::equal(m.det(), 42.0)) << "3.dat";
}

TEST(Matrix, Determinant4) {
    std::ifstream input("tests/matrix/unit_tests/determinant/determinant_cases/4.dat");
    matrix::matrix_t<double> m(50, 50);
    input >> m;
    ASSERT_TRUE(matrix::equal(m.det(), 1.0)) << "4.dat";
}

TEST(Matrix, Determinant5) {
    std::ifstream input("tests/matrix/unit_tests/determinant/determinant_cases/5.dat");
    matrix::matrix_t<double> m(100, 100);
    input >> m;
    ASSERT_TRUE(matrix::equal(m.det(), 42.0)) << "5.dat";
}