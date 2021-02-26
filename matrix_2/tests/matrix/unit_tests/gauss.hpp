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

TEST(Matrix, LinarSystemSolve1) {
    
    matrix::matrix_t<long double> system_right = {{2.0, 1.0, -1.0},
                                                  {3.0, 1.0, -2.0},
                                                  {1.0, 0.0, 1.0}};


    matrix::matrix_t<long double> system_left = {{2.0},
                                                 {3.0}, 
                                                 {3.0}};

    matrix::matrix_t<long double> partly_solution = {{2}, 
                                                     {-1}, 
                                                     {1}};
    matrix::matrix_t<long double> fundamental_matrix;

    auto ret = matrix::solve_linear_system(system_right, system_left);
    ASSERT_TRUE((ret.first == partly_solution) && (ret.second == fundamental_matrix)) << " joint system, the solution is unique"; 
}

TEST(Matrix, LinarSystemSolve2)  {
    matrix::matrix_t <long double> system_right = {{1.0, 3.0, 2.0, 2.0, 5.0},
                                                   {2.0, 2.0, 3.0, 2.0, 5.0},
                                                   {3.0, 1.0, 1.0, 2.0, 2.0},
                                                   {1.0, 1.0, -1.0, 1.0, 0.0}};

    matrix::matrix_t<long double> system_left = {{0.0},
                                                 {0.0},
                                                 {0.0},
                                                 {0.0}};                                               

    matrix::matrix_t<long double> partly_solution = {{0}, 
                                                     {0}, 
                                                     {0},
                                                     {0},
                                                     {0}};

    matrix::matrix_t<long double> fundamental_matrix = { {-0.5, 0 },
                                                         {-0.5, -1},
                                                         {0   , -1},
                                                         {1   , 0 },
                                                         {0   , 1 } };

    auto ret = matrix::solve_linear_system(system_right, system_left);
    ASSERT_TRUE((ret.first == partly_solution) && (ret.second == fundamental_matrix)) << " joint, homogeneous system m * n ";                                                     
}

TEST(Matrix, LinarSystemSolve3) {
    matrix::matrix_t<long double> system_right = {{1.0, 2.0, 3.0},
                                                  {3.0, 2.0, 1.0},
                                                  {4.0, 5.0, 6.0},
                                                  {6.0, 5.0, 4.0},
                                                  {10.0, 10.0, 10.0},
                                                  {0.0, 0.0, 0.0}};


    matrix::matrix_t<long double> system_left = {{1.0},
                                                 {2.0}, 
                                                 {3.0},
                                                 {4.0},
                                                 {5.0},
                                                 {6.0}};


    matrix::matrix_t<long double> partly_solution;
    matrix::matrix_t<long double> fundamental_matrix;

    auto ret = matrix::solve_linear_system(system_right, system_left);
    ASSERT_TRUE((ret.first == partly_solution) && (ret.second == fundamental_matrix)) << " !joint system  ";    
}

TEST(Matrix, LinarSystemSolve4)  {
    /* joint, !homogeneous system m * n */
    matrix::matrix_t <long double> system_right = {{6.0, 3.0, 14.0, -2.0, 1.0},
                                                   {20.0, 5.0, 10.0, 4.0, 11.0},
                                                   {13.0, 4.0, 12.0, 1.0, 6.0},
                                                   {4.0, 7.0, 46.0, -12.0, -7.0},
                                                   {1.0, -2.0, -16.0, 5.0, 4.0}};

    matrix::matrix_t<long double> system_left = {{2.0},
                                                 {20.0},
                                                 {11.0},
                                                 {-12.0},
                                                 {7.0}};            

    matrix::matrix_t<long double> partly_solution = {{ 5.0   / 3.0}, 
                                                     { -8.0  / 3.0}, 
                                                     {0.0},
                                                     {0.0},
                                                     {0.0}};
    matrix::matrix_t<long double> fundamental_matrix = { {  4.0 / 3.0  , -11.0 / 15.0 , -14.0 / 15.0 },
                                                         { -22.0 / 3.0 , 32.0 / 15.0 , 23.0 / 15.0  },
                                                         { 1           , 0           , 0            },
                                                         { 0           , 1           , 0            },
                                                         { 0           , 0           , 1            } };
    auto ret = matrix::solve_linear_system(system_right, system_left);
    ASSERT_TRUE((ret.first == partly_solution) && (ret.second == fundamental_matrix)) << " joint, !homogeneous system m * n ";                                                                 
}