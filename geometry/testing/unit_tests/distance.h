#pragma once

#include <gtest/gtest.h>
#include <sstream>
#include <string>

#include "../../include/geometry.h"

TEST(GeometryDistanceTest, SignedPointPlane) {
    {
        ivkg::plane_t plane{1, 2, 1, 4};
        ivkg::point_t<3> point{10, 15, 20};
        long double signed_distance = ivkg::signed_distance(point, plane);
        ASSERT_TRUE(ivkg::equal(signed_distance, 26.12789));
    }

    {
        ivkg::plane_t plane{1, 2, 1, -2};
        ivkg::point_t<3> point{-2, -4, -10};
        long double signed_distance = ivkg::signed_distance(point, plane);
        ASSERT_TRUE(ivkg::equal(signed_distance, -8.98146));
    }

    {
        ivkg::plane_t plane{1, 2, 1, -2};
        ivkg::point_t<3> point{2, 4, 10};
        long double signed_distance = ivkg::signed_distance(point, plane);
        ASSERT_TRUE(ivkg::equal(signed_distance,7.34846));
    }
}

TEST(GeometryDistanceTest, LineLine) {
    {
        ivkg::line_t<3> l1{ivkg::vector_t<3>{0, 0, 0}, ivkg::vector_t<3>{1, 0, 0}};
        ivkg::line_t<3> l2{ivkg::vector_t<3>{0, 0, 3}, ivkg::vector_t<3>{1, 0, 0}};
        long double distance = ivkg::distance(l1, l2);
        ASSERT_DOUBLE_EQ(distance, 3) << "parallel lines";
    }

    {
        ivkg::line_t<3> l1{ivkg::vector_t<3>{1, 1, 2}, ivkg::vector_t<3>{1, 0, 0}};
        ivkg::line_t<3> l2{ivkg::vector_t<3>{1, 1, 2}, ivkg::vector_t<3>{1, 0, 0}};
        long double distance = ivkg::distance(l1, l2);
        ASSERT_DOUBLE_EQ(distance, 0) << "equal lines";
    }

    {
        ivkg::line_t<3> l1{ivkg::vector_t<3>{1, 0, 1}, ivkg::vector_t<3>{1, 0, 0}};
        ivkg::line_t<3> l2{ivkg::vector_t<3>{1, 0, 0}, ivkg::vector_t<3>{0, 1, 0}};
        long double distance = ivkg::distance(l1, l2);
        ASSERT_DOUBLE_EQ(distance, 1);
    }

    {
        ivkg::line_t<3> l1{ivkg::vector_t<3>{2, 1, 2}, ivkg::vector_t<3>{1, 4, 7}};
        ivkg::line_t<3> l2{ivkg::vector_t<3>{1, 5, 2}, ivkg::vector_t<3>{1, 0, 3}};
        long double distance = ivkg::distance(l1, l2);
        ASSERT_TRUE(ivkg::equal(distance, 0.30151));
    }

    {
        ivkg::line_t<3> l1{ivkg::vector_t<3>{2, 1, 12}, ivkg::vector_t<3>{1, 1, 7}};
        ivkg::line_t<3> l2{ivkg::vector_t<3>{1, 5, 2}, ivkg::vector_t<3>{1, 6, 3}};
        long double distance = ivkg::distance(l1, l2);
        ASSERT_TRUE(ivkg::equal(distance, 0.12651));
    }
}

TEST(GeometryDistanceTest, SegmentSegment) {
    {
        ivkg::segment_t<3> s1{ivkg::point_t<3>{2.79, 6.44, 1.31}, ivkg::point_t<3>{1.56, 2.15, 2.45}};
        ivkg::segment_t<3> s2{ivkg::point_t<3>{-1.76, -2.71, 5.29}, ivkg::point_t<3>{-8.48, -6.76, 4.73}};
        long double distance = ivkg::distance(s1, s2);
        ivkg::point_t<3> A{1.56, 2.15, 2.45};
        ivkg::point_t<3> B{-1.76, -2.71, 5.29};
        ASSERT_TRUE(ivkg::equal(distance, A.distance(B)));
    }
}