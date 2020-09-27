#include <gtest/gtest.h>
#include <sstream>
#include <string>

#include "geometry.h"

TEST(GeometryPointTest, IOtest) {
    {
        std::istringstream in("1.0 2.0 3.0");
        ivkg::point_t<3> p; in >> p;
        ASSERT_DOUBLE_EQ(1.0, p[0]);
        ASSERT_DOUBLE_EQ(2.0, p[1]);
        ASSERT_DOUBLE_EQ(3.0, p[2]);
    }

    {
        std::ostringstream out;
        ivkg::point_t<3> p({1.1, 2.2, 3.3}); out << p;
        std::string expected = "(1.1, 2.2, 3.3)";
        std::string actual = out.str();
        ASSERT_EQ(expected, actual);
    }
}

TEST(GeometryPointTest, ValidTest) {
    {
        ivkg::point_t<10> x1;
        ASSERT_FALSE(x1.valid()) << "invalid point dim = 10";
    }

    {
        ivkg::point_t<3> x({1.0, 2.0, 3.0});
        ASSERT_TRUE(x.valid());
    }
}

TEST(GeometryPointTest, CompareTest) {
    {
        ivkg::point_t<3> x1({1.0, 5.0, 3.0});
        ivkg::point_t<3> x2({1.0, 5.0, 3.0});
        ASSERT_TRUE(x1 == x2);
    }

    {
        ivkg::point_t<3> x1({1.0, 2.0, 3.0});
        ivkg::point_t<3> x2({0.0, 0.0, 0.0});
        ASSERT_FALSE(x1 == x2);
    }

    {
        ivkg::point_t<3> x1({1.0, 2.0, 3.0});
        ivkg::point_t<3> x2;
        ASSERT_FALSE(x1 == x2) << "x2 - invalid point";
    }
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}