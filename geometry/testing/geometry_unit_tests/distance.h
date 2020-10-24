#pragma once

#include <gtest/gtest.h>
#include <sstream>
#include <string>

#include "../../src/include/geometry.h"

TEST(GeometryDistanceTest, SignedPointPlane) {
    {
        lingeo::plane_t plane{1, 2, 1, 4};
        lingeo::point_t<3> point{10, 15, 20};
        long double signed_distance = lingeo::signed_distance(point, plane);
        ASSERT_TRUE(std::abs(signed_distance - 26.12789) < 1e-5);
    }

    {
        lingeo::plane_t plane{1, 2, 1, -2};
        lingeo::point_t<3> point{-2, -4, -10};
        long double signed_distance = lingeo::signed_distance(point, plane);
        ASSERT_TRUE(std::abs(signed_distance - -8.98146) < 1e-5);
    }

    {
        lingeo::plane_t plane{1, 2, 1, -2};
        lingeo::point_t<3> point{2, 4, 10};
        long double signed_distance = lingeo::signed_distance(point, plane);
        ASSERT_TRUE(std::abs(signed_distance - 7.34846) < 1e-5);
    }
}

TEST(GeometryDistanceTest, LineLine) {
    {
        lingeo::line_t<3> l1{lingeo::vector_t<3>{0, 0, 0}, lingeo::vector_t<3>{1, 0, 0}};
        lingeo::line_t<3> l2{lingeo::vector_t<3>{0, 0, 3}, lingeo::vector_t<3>{1, 0, 0}};
        long double distance = lingeo::distance(l1, l2);
        ASSERT_DOUBLE_EQ(distance, 3) << "parallel lines";
    }

    {
        lingeo::line_t<3> l1{lingeo::vector_t<3>{1, 1, 2}, lingeo::vector_t<3>{1, 0, 0}};
        lingeo::line_t<3> l2{lingeo::vector_t<3>{1, 1, 2}, lingeo::vector_t<3>{1, 0, 0}};
        long double distance = lingeo::distance(l1, l2);
        ASSERT_DOUBLE_EQ(distance, 0) << "equal lines";
    }

    {
        lingeo::line_t<3> l1{lingeo::vector_t<3>{1, 0, 1}, lingeo::vector_t<3>{1, 0, 0}};
        lingeo::line_t<3> l2{lingeo::vector_t<3>{1, 0, 0}, lingeo::vector_t<3>{0, 1, 0}};
        long double distance = lingeo::distance(l1, l2);
        ASSERT_DOUBLE_EQ(distance, 1);
    }

    {
        lingeo::line_t<3> l1{lingeo::vector_t<3>{2, 1, 2}, lingeo::vector_t<3>{1, 4, 7}};
        lingeo::line_t<3> l2{lingeo::vector_t<3>{1, 5, 2}, lingeo::vector_t<3>{1, 0, 3}};
        long double distance = lingeo::distance(l1, l2);
        ASSERT_TRUE(std::abs(distance - 0.30151) < 1e-5);
    }

    {
        lingeo::line_t<3> l1{lingeo::vector_t<3>{2, 1, 12}, lingeo::vector_t<3>{1, 1, 7}};
        lingeo::line_t<3> l2{lingeo::vector_t<3>{1, 5, 2}, lingeo::vector_t<3>{1, 6, 3}};
        long double distance = lingeo::distance(l1, l2);
        ASSERT_TRUE(std::abs(distance - 0.12651) < 1e-5);
    }
}