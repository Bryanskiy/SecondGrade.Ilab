#pragma once

#include <gtest/gtest.h>
#include "../../../matrix/matrix.hpp"

TEST(Matrix, GaussStraight) {

    {
        /* m = n */
        matrix::matrix_t<double> m =   {{1, 2, 3},
                                        {4, 5, 6},
                                        {7, 8, 9}}; 

        matrix::gauss_straight(m);
        ASSERT_EQ(m[1][0], 0.0);
        ASSERT_EQ(m[2][0], 0.0); ASSERT_EQ(m[2][1], 0.0);
    }

    {
        /* m > n */
        matrix::matrix_t<double> m =   {{1, 2, 3},
                                        {4, 5, 6},
                                        {7, 8, 9},
                                        {10, 11, 12}};

        matrix::gauss_straight(m);
        ASSERT_EQ(m[1][0], 0.0);
        ASSERT_EQ(m[2][0], 0.0); ASSERT_EQ(m[2][1], 0.0);
        ASSERT_EQ(m[3][0], 0.0); ASSERT_EQ(m[3][1], 0.0); ASSERT_EQ(m[3][2], 0.0);                                     
    }

        {
        /* m < n */
        matrix::matrix_t<double> m =   {{1, 2, 3, 10},
                                        {4, 5, 6, 11},
                                        {7, 8, 9, 12}};

        matrix::gauss_straight(m);
        ASSERT_EQ(m[1][0], 0.0);
        ASSERT_EQ(m[2][0], 0.0); ASSERT_EQ(m[2][1], 0.0);                                
    }                                                       
}