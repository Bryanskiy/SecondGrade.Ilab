#pragma once

#include <gtest/gtest.h>
#include "../../matrix.h"

TEST(MatrixUnitTest, SelectorsGetters) {
    {
        matrix::matrix_t<int> m(100, 400, -2);

        ASSERT_EQ(m.get_row_number(), 100lu);
        ASSERT_EQ(m.get_col_number(), 400lu);
        ASSERT_EQ(m.get_elem_number(), 40000lu);
    }

    //change matrix
}

TEST(MatrixUnitTest, SelectorsTrace) {
    {
        matrix::matrix_t<int> m(5, 5, 0);

        ASSERT_EQ(m.get_row_number(), 5lu);
        ASSERT_EQ(m.get_col_number(), 5lu);
        ASSERT_EQ(m.get_elem_number(), 25lu);

        ASSERT_EQ(m.trace(), 0);
    }

    {
        matrix::matrix_t<int> m(6, 6, -1);

        ASSERT_EQ(m.get_row_number(), 6lu);
        ASSERT_EQ(m.get_col_number(), 6lu);
        ASSERT_EQ(m.get_elem_number(), 36lu);

        ASSERT_EQ(m.trace(), -6);
    }
}