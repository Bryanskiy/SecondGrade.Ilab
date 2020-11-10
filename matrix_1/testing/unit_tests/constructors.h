#pragma once

#include <gtest/gtest.h>
#include <vector>
#include "../../matrix.h"

TEST(MatrixUnitTest, ValueConstrictor) {
    {
        matrix::matrix_t<int> m(5, 4, 1);

        ASSERT_EQ(m.get_row_number(), 5lu);
        ASSERT_EQ(m.get_col_number(), 4lu);
        ASSERT_EQ(m.get_elem_number(), 20lu);

        for(std::size_t i = 0, maxi = m.get_row_number(); i < maxi; ++i) {
            for(std::size_t j = 0, maxj = m.get_col_number(); j < maxj; ++j) {
                ASSERT_EQ(m[i][j], 1);
            }
        }
    }

    {
        matrix::matrix_t<double> m(10, 4, 4.2);

        ASSERT_EQ(m.get_row_number(), 10lu);
        ASSERT_EQ(m.get_col_number(), 4lu);
        ASSERT_EQ(m.get_elem_number(), 40lu);

        for(std::size_t i = 0, maxi = m.get_row_number(); i < maxi; ++i) {
            for(std::size_t j = 0, maxj = m.get_col_number(); j < maxj; ++j) {
                ASSERT_TRUE(std::abs(m[i][j] - 4.2) < 1e-5);
            }
        }
    }
}

TEST(MatrixUnitTest, IteratorConstrictor) {
    {
        std::vector<int> v{1, 1, 1, 2, 2, 2};
        matrix::matrix_t<int> m(2, 3, v.begin(), v.end());

        ASSERT_EQ(m.get_row_number(), 2lu);
        ASSERT_EQ(m.get_col_number(), 3lu);
        ASSERT_EQ(m.get_elem_number(), 6lu);

        for(std::size_t i = 0, maxi = m.get_row_number(); i < maxi; ++i) {
            for(std::size_t j = 0, maxj = m.get_col_number(); j < maxj; ++j) {
                ASSERT_EQ(m[i][j], i + 1);
            }
        }
    }
}

TEST(MatrixUnitTest, CopyConstructor) {
    {
        matrix::matrix_t<int> m1(5, 4, 1);
        matrix::matrix_t<int> m2 = m1;

        ASSERT_EQ(m2.get_row_number(), 5lu);
        ASSERT_EQ(m2.get_col_number(), 4lu);
        ASSERT_EQ(m2.get_elem_number(), 20lu);

        ASSERT_EQ(m2.get_row_number(), m1.get_row_number());
        ASSERT_EQ(m2.get_col_number(), m1.get_col_number());
        ASSERT_EQ(m2.get_elem_number(), m1.get_elem_number());

        for(std::size_t i = 0, maxi = m2.get_row_number(); i < maxi; ++i) {
            for(std::size_t j = 0, maxj = m2.get_col_number(); j < maxj; ++j) {
                ASSERT_EQ(m2[i][j], 1);
                ASSERT_EQ(m2[i][j], m1[i][j]);
            }
        }
    }
}