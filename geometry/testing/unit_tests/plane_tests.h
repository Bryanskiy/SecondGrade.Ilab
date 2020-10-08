#pragma once

#include <gtest/gtest.h>
#include <sstream>
#include <string>

#include "../../include/geometry.h"

TEST(GeometryPlaneTest, Intersection) {
    {
        ivkg::plane_t a({2, 0, 0, 5});
        ivkg::plane_t b({1, 4, 0, 0});
        ivkg::line_t<3> intersection = a.intersect(b);
        ASSERT_DOUBLE_EQ(intersection.get_start()[0] * 2 + 5.0, 0.0);
        ASSERT_DOUBLE_EQ(intersection.get_start()[0] + intersection.get_start()[1] * 4.0, 0.0);
    }

    {
        ivkg::plane_t a({1, 1, 1, 2});
        ivkg::plane_t b({1, 3, 2, 0});
        ivkg::line_t<3> intersection = a.intersect(b);
        ASSERT_TRUE(ivkg::equal(intersection.get_start()[0] * 1.0 + intersection.get_start()[1] * 1.0 +
                                     intersection.get_start()[2] * 1.0 + 2.0, 0.0));
        ASSERT_TRUE(ivkg::equal(intersection.get_start()[0] * 1 + intersection.get_start()[1] * 3.0 +
                                     intersection.get_start()[3] * 2, 0.0));
    }

    {
        ivkg::plane_t a({1, 1, 3, 2});
        ivkg::plane_t b({1, 3, 2, 2});
        ivkg::line_t<3> intersection = a.intersect(b);
        ASSERT_TRUE(std::abs(intersection.get_start()[0] * 1.0 + intersection.get_start()[1] * 1.0 +
                                     intersection.get_start()[2] * 3.0 + 2.0) < 1e-5);
        ASSERT_TRUE(std::abs(intersection.get_start()[0] * 1 + intersection.get_start()[1] * 3.0 +
                                     intersection.get_start()[2] * 2 + 2.0) < 1e-5);
    }
}

TEST(GeometryPlaneTest, ConstructFromPoints) {
    {
        ivkg::point_t<3> x1({1.0, 2.0, 3.0});
        ivkg::point_t<3> x2({1.0, 2.0, 4.0});
        ivkg::point_t<3> x3({4.0, 2.0, -1.0});

        ivkg::plane_t p(x1, x2, x3);
        ASSERT_DOUBLE_EQ(0.0, p[0]);
        ASSERT_DOUBLE_EQ(3.0, p[1]);
        ASSERT_DOUBLE_EQ(0.0, p[2]);
        ASSERT_DOUBLE_EQ(-6.0, p[3]);
    }

    {
        ivkg::point_t<3> x1({-3.0, 2.0, -1.0});
        ivkg::point_t<3> x2({-1.0, 2.0, 4.0});
        ivkg::point_t<3> x3({3.0, 3.0, -1.0});

        ivkg::plane_t p(x1, x2, x3);
        ASSERT_DOUBLE_EQ(-5.0, p[0]);
        ASSERT_DOUBLE_EQ(30.0, p[1]);
        ASSERT_DOUBLE_EQ(2.0, p[2]);
        ASSERT_DOUBLE_EQ(-73.0, p[3]);
    }

}

TEST(GeometryPlaneTest, Equal) {
    {
        ivkg::plane_t p1({1.0, 2.0, 0.0, 3.0});
        ivkg::plane_t p2({2.0, 4.0, 0.0, 6.0});
        ASSERT_TRUE(p1 == p2);
    }

    {
        ivkg::plane_t p1({1.0, 2.0, 0.0, 2.0});
        ivkg::plane_t p2({2.0, 4.0, 0.0, 6.0});
        ASSERT_FALSE(p1 == p2);
    }
}

TEST(GeometryPlaneTest, IsParallel) {
    {
        ivkg::plane_t p1({1.0, 1.0, 1.0, 0.0});
        ivkg::plane_t p2({2.0, 2.0, 2.0, 5.0});
        ASSERT_TRUE(parallel(p1, p2));
    }

    {
        ivkg::plane_t p1({1.0, 1.0, 1.0, 0.0});
        ivkg::plane_t p2({3.0, 2.0, 2.0, 5.0});
        ASSERT_FALSE(parallel(p1, p2));
    }

}