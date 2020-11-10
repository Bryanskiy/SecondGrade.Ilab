#pragma once

#include <gtest/gtest.h>
#include <vector>
#include "../../matrix.h"

TEST(MatrixUnitTest, OperatorsEqual) {
    {
        matrix::matrix_t<int> m1(5, 5, 0);
        matrix::matrix_t<int> m2(5, 5, 0);

        ASSERT_TRUE(m1 == m2) << "2 valid and equal matrices";
    }

    {
        matrix::matrix_t<int> m1;
        matrix::matrix_t<int> m2(5, 5, 0);

        ASSERT_FALSE(m1 == m2) << "first invalid matrix";
    }

    {
        matrix::matrix_t<int> m1;
        matrix::matrix_t<int> m2(5, 5, 0);

        ASSERT_FALSE(m2 == m1) << "second invalid matrix";
    }

    {
        matrix::matrix_t<int> m1;
        matrix::matrix_t<int> m2;

        ASSERT_FALSE(m1 == m2) << "2 invalid matrices";
    }

    {
        matrix::matrix_t<int> m1;
        ASSERT_FALSE(m1 == m1) << "invalid matrix equal itself";
    }

    {
        matrix::matrix_t<int> m(5, 5, 0);
        ASSERT_TRUE(m == m) << "valid matrix equal itself";
    }
}

TEST(MatrixUnitTest, OperatorsAssigment) {
    {
        matrix::matrix_t<int> m1(13, 7, 3);
        matrix::matrix_t<int> m2;
        m2 = m1;

        ASSERT_TRUE(m2 == m1);
    }
}

TEST(MatrixUnitTest, OperatorsBrackets) {
    {
        std::vector<int> v{1, 2, 5, 12, -2, 0};
        matrix::matrix_t<int> m(3, 2, v.begin(), v.end());

        ASSERT_TRUE(m[0][0] == 1);
        ASSERT_TRUE(m[0][1] == 2);
        ASSERT_TRUE(m[1][0] == 5);
        ASSERT_TRUE(m[1][1] == 12);
        ASSERT_TRUE(m[2][0] == -2);
        ASSERT_TRUE(m[2][1] == 0);
    }
}