#pragma once

#include <gtest/gtest.h>
#include "../../../matrix/matrix.hpp"

TEST(Matrix, Resize) {
    {
        matrix::matrix_t<int> m = { {1, 2, 3}, 
                                    {4, 5, 6}, 
                                    {7, 8, 9} };

        m.resize(2, 2);
        ASSERT_TRUE(m[0][0] == 1);
        ASSERT_TRUE(m[0][1] == 2);
        ASSERT_TRUE(m[1][0] == 4);
        ASSERT_TRUE(m[1][1] == 5);
        ASSERT_TRUE(m.get_cols_number() == 2);
        ASSERT_TRUE(m.get_rows_number() == 2);
        ASSERT_TRUE(m.get_elements_number() == 4);                     
    }

    {
        matrix::matrix_t<int> m = { {1, 2, 3}, 
                                    {4, 5, 6}, 
                                    {7, 8, 9} };

        m.resize(4, 4);
        ASSERT_TRUE(m[0][0] == 1); ASSERT_TRUE(m[0][1] == 2); ASSERT_TRUE(m[0][2] == 3); ASSERT_TRUE(m[0][3] == 0);
        ASSERT_TRUE(m[1][0] == 4); ASSERT_TRUE(m[1][1] == 5); ASSERT_TRUE(m[1][2] == 6); ASSERT_TRUE(m[1][3] == 0);
        ASSERT_TRUE(m[2][0] == 7); ASSERT_TRUE(m[2][1] == 8); ASSERT_TRUE(m[2][2] == 9); ASSERT_TRUE(m[2][3] == 0);
        ASSERT_TRUE(m[3][0] == 0); ASSERT_TRUE(m[3][1] == 0); ASSERT_TRUE(m[3][2] == 0); ASSERT_TRUE(m[3][3] == 0);
        ASSERT_TRUE(m.get_cols_number() == 4);
        ASSERT_TRUE(m.get_rows_number() == 4);
        ASSERT_TRUE(m.get_elements_number() == 16);   
    }

    {
        matrix::matrix_t<int> m = { {1, 2, 3}, 
                                    {4, 5, 6}, };

        m.resize(1, 4);
        ASSERT_TRUE(m[0][0] == 1); ASSERT_TRUE(m[0][1] == 2); ASSERT_TRUE(m[0][2] == 3); ASSERT_TRUE(m[0][3] == 0);
        ASSERT_TRUE(m.get_cols_number() == 4);
        ASSERT_TRUE(m.get_rows_number() == 1);
        ASSERT_TRUE(m.get_elements_number() == 4);   
    }
}