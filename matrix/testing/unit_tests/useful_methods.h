#pragma once

#include <gtest/gtest.h>
#include "../../matrix.h"

TEST(MatrixUnitTest, MethodsNegate) {
    {
        matrix::matrix_t<int> m(12, 5, -5);
        m.negate();
        for(std::size_t i = 0, maxi = m.get_col_number(); i < maxi; ++i) {
            for(std::size_t j = 0, maxj = m.get_row_number(); j < maxj; ++j) {
                ASSERT_TRUE(m[i][j] == 5);
            }
        }
    }

    {
        matrix::matrix_t<int> m(12, 5, 5);
        m.negate();
        for(std::size_t i = 0, maxi = m.get_col_number(); i < maxi; ++i) {
            for(std::size_t j = 0, maxj = m.get_row_number(); j < maxj; ++j) {
                ASSERT_TRUE(m[i][j] == -5);
            }
        }
    }
}

TEST(MatrixUnitTest, MethodsTranscpose) {
    {
        std::vector<int> v{1, 2, 5, 12, -2, 0};
        matrix::matrix_t<int> m(3, 2, v.begin(), v.end());

        m.transpose();

        ASSERT_EQ(m.get_row_number(), 2lu);
        ASSERT_EQ(m.get_col_number(), 3lu);
        ASSERT_EQ(m.get_elem_number(), 6lu);

        ASSERT_EQ(m[0][0], 1);
        ASSERT_EQ(m[0][1], 5);
        ASSERT_EQ(m[0][2], -2);
        ASSERT_EQ(m[1][0], 2);
        ASSERT_EQ(m[1][1], 12);
        ASSERT_EQ(m[1][2], 0);
    }
}

TEST(MatrixUnitTest, MethodsMaxAbsColElem) {
    {
        std::vector<int> v{1, 2, 5, 12, -2, 0};
        matrix::matrix_t<int> m(3, 2, v.begin(), v.end());

        ASSERT_EQ(m[m.max_abs_col_elem(0)][0], 5);
        ASSERT_EQ(m[m.max_abs_col_elem(1)][1], 12);
    }

    {
        std::vector<int> v{1, 2, 5, 12, -2, 0};
        matrix::matrix_t<int> m(2, 3, v.begin(), v.end());

        ASSERT_EQ(m[m.max_abs_col_elem(0)][0], 12);
        ASSERT_EQ(m[m.max_abs_col_elem(1)][1], 2);
        ASSERT_EQ(m[m.max_abs_col_elem(1)][2], 5);
    }
}

TEST(MatrixUnitTest, MethodsSwapRows) {
    {
        std::vector<int> actual_v{1, 2, 5, 12, -2, 0};
        std::vector<int> expected1_v{5, 12, 1, 2, -2, 0};
        std::vector<int> expected2_v{1, 2, 5, 12, -2, 0};

        matrix::matrix_t<int> actual_m(3, 2, actual_v.begin(), actual_v.end());
        matrix::matrix_t<int> expected1_m(3, 2, expected1_v.begin(), expected1_v.end());
        matrix::matrix_t<int> expected2_m(3, 2, expected2_v.begin(), expected2_v.end());

        actual_m.swap_rows(0, 1);
        ASSERT_EQ(actual_m, expected1_m);

        actual_m.swap_rows(0, 1);
        ASSERT_EQ(actual_m, expected2_m);
    }

    {
        std::vector<int> actual_v{1, 2, 5, 12, -2, 0};
        std::vector<int> expected1_v{1, 2, -2, 0, 5, 12};
        std::vector<int> expected2_v{1, 2, 5, 12, -2, 0};

        matrix::matrix_t<int> actual_m(3, 2, actual_v.begin(), actual_v.end());
        matrix::matrix_t<int> expected1_m(3, 2, expected1_v.begin(), expected1_v.end());
        matrix::matrix_t<int> expected2_m(3, 2, expected2_v.begin(), expected2_v.end());

        actual_m.swap_rows(1, 2);
        ASSERT_EQ(actual_m, expected1_m);

        actual_m.swap_rows(1, 2);
        ASSERT_EQ(actual_m, expected2_m);
    }

    {
        std::vector<int> actual_v{1, 2, 5, 12, -2, 0};
        std::vector<int> expected1_v{1, 2, 5, 12, -2, 0};
        std::vector<int> expected2_v{1, 2, 5, 12, -2, 0};

        matrix::matrix_t<int> actual_m(3, 2, actual_v.begin(), actual_v.end());
        matrix::matrix_t<int> expected1_m(3, 2, expected1_v.begin(), expected1_v.end());
        matrix::matrix_t<int> expected2_m(3, 2, expected2_v.begin(), expected2_v.end());

        actual_m.swap_rows(1, 1);
        ASSERT_EQ(actual_m, expected1_m);

        actual_m.swap_rows(1, 1);
        ASSERT_EQ(actual_m, expected2_m);
    }
}

TEST(MatrixUnitTest, MethodsGaussStraight) {
    {
        std::vector<int> actual_v{1, 1, 0, 1};
        matrix::matrix_t<int> actual_m(2, 2, actual_v.begin(), actual_v.end());

        std::vector<int> expected_v{1, 1, 0, 1};
        matrix::matrix_t<int> expected_m(2, 2, expected_v.begin(), expected_v.end());

        actual_m.gauss_straight();

        ASSERT_EQ(actual_m, expected_m);
    }

    {
        std::vector<int> actual_v{1, 1, 1, 1};
        matrix::matrix_t<int> actual_m(2, 2, actual_v.begin(), actual_v.end());

        std::vector<int> expected_v{1, 1, 0, 0};
        matrix::matrix_t<int> expected_m(2, 2, expected_v.begin(), expected_v.end());

        actual_m.gauss_straight();

        ASSERT_EQ(actual_m, expected_m);
    }

    {
        std::vector<int> actual_v{1, 2, 5, 12};
        matrix::matrix_t<int> actual_m(2, 2, actual_v.begin(), actual_v.end());

        std::vector<int> expected_v{5, 12, 0, -2};
        matrix::matrix_t<int> expected_m(2, 2, expected_v.begin(), expected_v.end());

        actual_m.gauss_straight();

        ASSERT_EQ(actual_m, expected_m);
    }

    {
        std::vector<int> actual_v{3, 5, 7, 8, 6, 4, 2, 1};
        matrix::matrix_t<int> actual_m(2, 4, actual_v.begin(), actual_v.end());

        std::vector<int> expected_v{6, 4, 2, 1, 0, 18, 36, 45};
        matrix::matrix_t<int> expected_m(2, 4, expected_v.begin(), expected_v.end());

        actual_m.gauss_straight();

        ASSERT_EQ(actual_m, expected_m);
    }

    {
        std::vector<int> actual_v{3, 5, 7, 8, 6, 4, 2, 1, 9, 7, 3, 3};
        matrix::matrix_t<int> actual_m(3, 4, actual_v.begin(), actual_v.end());

        std::vector<int> expected_v{9, 7, 3, 3, 0, 24, 54, 63, 0, 0, 324, 162};
        matrix::matrix_t<int> expected_m(3, 4, expected_v.begin(), expected_v.end());

        actual_m.gauss_straight();

        ASSERT_EQ(actual_m, expected_m);
    }
}

TEST(MatrixUnitTest, MethodsDeterminant) {
    {
        std::vector<int> v{1, 2, 5, 12};
        matrix::matrix_t<int> m(2, 2, v.begin(), v.end());

        ASSERT_EQ(m.det(), 2);
    }
}