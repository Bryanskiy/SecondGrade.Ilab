#pragma once

#include <gtest/gtest.h>
#include <vector>
#include <optional>
#include "../../matrix.h"

TEST(MatrixUnitTest, FuncsMultiplication1) {
    {
        std::vector<int> v1{1, 2, 2, 1};
        std::vector<int> v2{2, 1, 4, 2};
        std::vector<int> expected_v{10, 5, 8, 4};

        matrix::matrix_t<int> m1 = {2, 2, v1.begin(), v2.end()};
        matrix::matrix_t<int> m2 = {2, 2, v2.begin(), v2.end()};
        matrix::matrix_t<int> expected_m = {2, 2, expected_v.begin(), expected_v.end()};

        matrix::matrix_t<int> actual = matrix::multiplication(m1, m2);
        ASSERT_TRUE(actual == expected_m);
    }

    {
        std::vector<int> v1{1, -1, 2, 5, 1, 6, 7, -1, 2};
        std::vector<int> v2{4, 2, 1};
        std::vector<int> expected_v{4, 28, 28};

        matrix::matrix_t<int> m1 = {3, 3, v1.begin(), v2.end()};
        matrix::matrix_t<int> m2 = {3, 1, v2.begin(), v2.end()};
        matrix::matrix_t<int> expected_m = {3, 1, expected_v.begin(), expected_v.end()};

        matrix::matrix_t<int> actual = matrix::multiplication(m1, m2);
        ASSERT_EQ(actual.get_col_number(), 1);
        ASSERT_EQ(actual.get_row_number(), 3);
    }
}
