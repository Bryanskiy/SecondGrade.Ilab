#pragma once

#include <gtest/gtest.h>
#include <sstream>
#include <string>

#include "../../src/include/geometry.h"

TEST(GeometryIntersectionTest, TriangleTriangle) {
    //case 1
    {
        ivkg::point_t<3> A{-0.83, 2.29, 12.94};
        ivkg::point_t<3> E{-4, 2, 0};
        ivkg::point_t<3> F{-3.61, 4.02, 0};

        ivkg::point_t<3> B{2.77, -1.46, 2.45};
        ivkg::point_t<3> C{4.19, 0.47, 5.17};
        ivkg::point_t<3> D{-1.76, -2.71, 0.4};

        ivkg::triangle_t<3> tr1{A, E, F};
        ivkg::triangle_t<3> tr2{B, C, D};

        ASSERT_FALSE(ivkg::intersection(tr1, tr2)) << "case 1: (1) triangles do not intersect, "
                                                                       "(2) don't lie on 1 plane, "
                                                                       "(3)don't intersect their planes";
    }

    //case 2
    {
        ivkg::point_t<3> A{-0.44, -2.01, 14.98};
        ivkg::point_t<3> E{-2.51, -0.46, 6.41};
        ivkg::point_t<3> F{-4.47, 9.81, 5.35};

        ivkg::point_t<3> B{3.21, -2.76, 11.04};
        ivkg::point_t<3> C{-1.19, 12.34, 14.77};
        ivkg::point_t<3> D{7.99, 7.38, 10};

        ivkg::triangle_t<3> tr1{A, E, F};
        ivkg::triangle_t<3> tr2{B, C, D};

        ASSERT_FALSE(ivkg::intersection(tr1, tr2)) << "case 2: (1) triangles do not intersect, "
                                                                       "(2) don't lie on 1 plane, "
                                                                       "(3) 1 triangle intersect other plane";
    }

    //case 3
    {
        ivkg::point_t<3> A{0, 0, 0};
        ivkg::point_t<3> E{-7, 0.05, 0.93};
        ivkg::point_t<3> F{0, -11, 0};

        ivkg::point_t<3> B{-15.49, -17.11, -1.33};
        ivkg::point_t<3> C{0.87, 8.27, -0.82};
        ivkg::point_t<3> D{7.99, 7.38, 8};

        ivkg::triangle_t<3> tr1{A, E, F};
        ivkg::triangle_t<3> tr2{B, C, D};

        ASSERT_TRUE(ivkg::intersection(tr1, tr2)) << "case 3: (1) triangles intersect, "
                                                                      "(2) dt1 in dt2, "
                                                                      "(3) don't lie on 1 plane";
    }

    //case 4
    {
        ivkg::point_t<3> A{0, 0, 2};
        ivkg::point_t<3> E{-10, 0, 0};
        ivkg::point_t<3> F{0, -11, 0};

        ivkg::point_t<3> B{-1.59, -8.25, -1.17};
        ivkg::point_t<3> C{-11.16, 4.32, -2.28};
        ivkg::point_t<3> D{-0.99, 2.08, 8};

        ivkg::triangle_t<3> tr1{A, E, F};
        ivkg::triangle_t<3> tr2{B, C, D};

        ASSERT_TRUE(ivkg::intersection(tr1, tr2)) << "case 4: (1) triangles intersect, "
                                                                      "(2) don't lie on 1 plane";
    }

    //case 5
    {
        ivkg::point_t<3> A{1.81, -1.4, 0};
        ivkg::point_t<3> E{-10.91, -1.67, 0};
        ivkg::point_t<3> F{-2.63, -11, 0};

        ivkg::point_t<3> B{-3.01, -3.2, 0};
        ivkg::point_t<3> C{-12.86, -5.27, 0};
        ivkg::point_t<3> D{-0.98, 2.08, 0};

        ivkg::triangle_t<3> tr1{A, E, F};
        ivkg::triangle_t<3> tr2{B, C, D};

        ASSERT_TRUE(ivkg::intersection(tr1, tr2)) << "case 5: (1) triangles intersect, "
                                                                      "(2) lie on 1 plane";
    }

    //case 6
    {
        ivkg::point_t<3> A{1.81, -1.4, 0};
        ivkg::point_t<3> E{-7.4, -7.33, 0};
        ivkg::point_t<3> F{-2.63, -11, 0};

        ivkg::point_t<3> B{-3.01, -3.2, 0};
        ivkg::point_t<3> C{-12.86, -5.27, 0};
        ivkg::point_t<3> D{-0.98, 2.08, 0};

        ivkg::triangle_t<3> tr1{A, E, F};
        ivkg::triangle_t<3> tr2{B, C, D};

        ASSERT_FALSE(ivkg::intersection(tr1, tr2)) << "case 6: (1) triangles don't intersect, "
                                                                       "(2) lie on 1 plane";
    }

    //case 7
    {
        ivkg::point_t<3> A{33.3363, 91.2551, 67.8482};
        ivkg::point_t<3> E{33.2543, 90.9706, 67.1548};
        ivkg::point_t<3> F{33.6565, 90.6161, 67.7456};

        ivkg::point_t<3> B{33.306, 91.067, 67.2561};
        ivkg::point_t<3> C{33.9626, 91.3808, 66.8995};
        ivkg::point_t<3> D{33.5757, 90.5551, 67.4595};

        ivkg::triangle_t<3> tr1{A, E, F};
        ivkg::triangle_t<3> tr2{B, C, D};

        ASSERT_FALSE(ivkg::intersection(tr1, tr2));
    }

    //case 8
    {
        ivkg::point_t<3> A{24.4199, 39.3683, 88.3612};
        ivkg::point_t<3> E{24.2155, 38.4732, 88.1412};
        ivkg::point_t<3> F{24.1576, 38.646, 88.6942};

        ivkg::point_t<3> B{24.8894, 42.4183, 87.4641};
        ivkg::point_t<3> C{25.4858, 42.5169, 88.01};
        ivkg::point_t<3> D{24.805, 42.0086, 87.528};

        ivkg::triangle_t<3> tr1{A, E, F};
        ivkg::triangle_t<3> tr2{B, C, D};

        ASSERT_FALSE(ivkg::intersection(tr1, tr2));
    }
}