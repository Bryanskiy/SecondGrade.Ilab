#pragma once

#include <gtest/gtest.h>
#include <sstream>
#include <string>

#include "../../src/include/geometry.h"

TEST(GeometryPointTest, ValidTest) {
    {
        lingeo::point_t<10> x1;
        ASSERT_FALSE(x1.valid()) << "invalid point dim = 10";
    }

    {
        lingeo::point_t<3> x({1.0, 2.0, 3.0});
        ASSERT_TRUE(x.valid()) << "valid point";
    }
}

TEST(GeometryPointTest, CompareTest) {
    {
        lingeo::point_t<3> x1{1.0, 5.0, 3.0};
        lingeo::point_t<3> x2{1.0, 5.0, 3.0};
        ASSERT_TRUE(x1 == x2) << "valid points";
    }

    {
        lingeo::point_t<3> x1{1.0, 2.0, 3.0};
        lingeo::point_t<3> x2{0.0, 0.0, 0.0};
        ASSERT_FALSE(x1 == x2) << "valid points";
    }

    {
        lingeo::point_t<3> x1({1.0, 2.0, 3.0});
        lingeo::point_t<3> x2;
        ASSERT_FALSE(x1 == x2) << "x2 - invalid point";
    }

    {
        lingeo::point_t<3> x1;
        lingeo::point_t<3> x2;
        ASSERT_FALSE(x1 == x2) << "x1, x2 - invalid point";
    }
}

TEST(GeometryPointTest, Distance) {
    {
        lingeo::point_t<3> x1{1.0, 5.0, 3.0};
        lingeo::point_t<3> x2{10.0, 5.0, 4.0};
        ASSERT_TRUE(lingeo::equal(x1.distance(x2), 9.05538)) << "valid points";
    }

    {
        lingeo::point_t<3> x1{1.0, 5.0, 3.0};
        lingeo::point_t<3> x2;
        ASSERT_FALSE(lingeo::valid(x1.distance(x2)));
    }

    {
        lingeo::point_t<3> x2{1.0, 5.0, 3.0};
        lingeo::point_t<3> x1;
        ASSERT_FALSE(lingeo::valid(x1.distance(x2)));
    }

    {
        lingeo::point_t<3> x1;
        lingeo::point_t<3> x2;
        ASSERT_FALSE(lingeo::valid(x1.distance(x2)));
    }
}

TEST(GeometryPointTest, IOtest) {
    {
        std::istringstream in("1.0 2.0 3.0");
        lingeo::point_t<3> p; in >> p;
        ASSERT_DOUBLE_EQ(1.0, p[0]) << "valid input";
        ASSERT_DOUBLE_EQ(2.0, p[1]) << "valid input";
        ASSERT_DOUBLE_EQ(3.0, p[2]) << "valid input";
    }

    {
        std::ostringstream out;
        lingeo::point_t<3> p{1.1, 2.2, 3.3}; out << p;
        std::string expected = "(1.1, 2.2, 3.3)";
        std::string actual = out.str();
        ASSERT_EQ(expected, actual) << "valid output";
    }
}