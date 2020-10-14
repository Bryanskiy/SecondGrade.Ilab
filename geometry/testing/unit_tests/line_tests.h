#pragma once

#include <gtest/gtest.h>
#include <sstream>
#include <string>

#include "../../src/include/geometry.h"

// todo : check if line construct from 2 equal points
TEST(GeometryLineTest, Valid) {
    {
        ivkg::line_t<100> line;
        ASSERT_FALSE(line.valid());
    }

    {
        ivkg::point_t<3> p1({0.0, 1.0, 2.0});
        ivkg::point_t<3> p2;
        ivkg::line_t<3> line(p1, p2);
        ASSERT_FALSE(line.valid()) << "construct from 2 points, 1 point invalid";
    }

    {
        ivkg::point_t<3> p1({0.0, 1.0, 2.0});
        ivkg::point_t<3> p2({1.0, 3.0, 4.0});
        ivkg::line_t<3> line(p1, p2);
        ASSERT_TRUE(line.valid());
    }
}

TEST(GeometryLineTest, Isparallel) {
    {
        ivkg::point_t<3> p1({0.0, 0.0, 0.0});
        ivkg::point_t<3> p2({1.0, 2.0, 0.0});
        ivkg::line_t<3> l1(p1, p2);

        ivkg::point_t<3> p3({0.0, 0.0, 0.0});
        ivkg::point_t<3> p4({2.0, 4.0, 0.0});
        ivkg::line_t<3> l2(p3, p4);

        ASSERT_TRUE(parallel(l1, l2));
    }

    {
        ivkg::point_t<3> p1({0.0, 0.0, 0.0});
        ivkg::point_t<3> p2({1.0, 2.0, 0.0});
        ivkg::line_t<3> l1(p1, p2);

        ivkg::point_t<3> p3({0.0, 0.0, 0.0});
        ivkg::point_t<3> p4({2.0, 5.0, 0.0});
        ivkg::line_t<3> l2(p3, p4);

        ASSERT_FALSE(parallel(l1, l2));
    }

    {
        ivkg::point_t<3> p1({0.0, 0.0, 0.0});
        ivkg::point_t<3> p2({1.0, 2.0, 0.0});
        ivkg::line_t<3> l1(p1, p2);

        ivkg::line_t<3> l2;

        ASSERT_FALSE(parallel(l1, l2)) << "one line is invalid";
    }
}
