#include <gtest/gtest.h>
#include <sstream>
#include <string>

#include "geometry.h"

/* ------------------------------------------------
                START POINT_TESTS
 -------------------------------------------------*/
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
        ivkg::point_t<3> p{1.1, 2.2, 3.3}; out << p;
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
        ivkg::point_t<3> x1{1.0, 5.0, 3.0};
        ivkg::point_t<3> x2{1.0, 5.0, 3.0};
        ASSERT_TRUE(x1 == x2);
    }

    {
        ivkg::point_t<3> x1{1.0, 2.0, 3.0};
        ivkg::point_t<3> x2{0.0, 0.0, 0.0};
        ASSERT_FALSE(x1 == x2);
    }

    {
        ivkg::point_t<3> x1({1.0, 2.0, 3.0});
        ivkg::point_t<3> x2;
        ASSERT_FALSE(x1 == x2) << "x2 - invalid point";
    }
}

/* ------------------------------------------------
                START VECTOR_TESTS
 -------------------------------------------------*
*/
TEST(GeometryVectorTest, Arithmetic) {
    {
        ivkg::vector_t<3> a{1.0, 2.0, 3.0};
        ivkg::vector_t<3> b{0.0, 4.0, 3.0};
        ivkg::vector_t<3> expected{1.0, 6.0, 6.0};
        ASSERT_EQ(a + b, expected);
        ASSERT_EQ(a += b, expected);
    }

    {
        ivkg::vector_t<3> a{1.0, 2.0, 3.0};
        ivkg::vector_t<3> b{0.0, 4.0, 3.0};
        ivkg::vector_t<3> expected{1.0, -2.0, 0.0};
        ASSERT_EQ(a - b, expected);
        ASSERT_EQ(a -= b, expected);
    }

    {
        ivkg::vector_t<3> a{1.0, 2.0, 3.0};
        ivkg::vector_t<3> a_copy{1.0, 2.0, 3.0};
        ivkg::vector_t<3> expected{-1.0, -2.0, -3.0};
        ASSERT_EQ(-a, expected);
        ASSERT_EQ(a, a_copy) << "unary sub modify vector";
    }

    {
        ivkg::vector_t<3> a{1.0, 2.0, 3.0};
        double lambda = 4.0;
        ivkg::vector_t<3> expected{4.0, 8.0, 12.0};
        ASSERT_EQ(a * lambda, expected) << "mult by positive lambda";
        ASSERT_EQ(a *= lambda, expected) << "mult by positive lambda";
    }

    {
        ivkg::vector_t<3> a{1.0, 2.0, 0.0};
        double lambda = 0.0;
        ivkg::vector_t<3> expected{0.0, 0.0, 0.0};
        ASSERT_EQ(a * lambda, expected) << "mult by zero lambda";
        ASSERT_EQ(a *= lambda, expected) << "mult by zero lambda";
    }

    {
        ivkg::vector_t<3> a{1.0, 2.0, 0.0};
        double lambda = -1.0;
        ivkg::vector_t<3> expected{-1.0, -2.0, 0.0};
        ASSERT_EQ(a * lambda, expected) << "mult by negative lambda";
        ASSERT_EQ(a *= lambda, expected) << "mult by negative lambda";
    }
}

TEST(GeometryVectorTest, Valid) {
    {
        ivkg::vector_t<10> x;
        ASSERT_FALSE(x.valid()) << "default construct";
    }

    {
        ivkg::point_t<3> x1{1.0, 2.0, 3.0};
        ivkg::point_t<3> x2{1.0, 3.0, 5.0};
        ivkg::vector_t<3> x{x1, x2};
        ASSERT_TRUE(x.valid()) << "construct from 2 valid points";
    }

    {
        ivkg::point_t<3> x1;
        ivkg::point_t<3> x2{1.0, 3.0, 5.0};
        ivkg::vector_t<3> x{x1, x2};
        ASSERT_FALSE(x.valid()) << "construct from 1 invalid and 1 valid points";
    }

    {
        ivkg::point_t<3> x1;
        ivkg::point_t<3> x2;
        ivkg::vector_t<3> x(x1, x2);
        ASSERT_FALSE(x.valid()) << "construct from 2 invalid points";
    }

    {
        ivkg::vector_t<3> x{1.0, 2.0, 3.0};
        ASSERT_TRUE(x.valid()) << "construct doubles vector";
    }

}

TEST(GeometryVectorTest, Len) {
    {
        ivkg::vector_t<3> v{3.0, 4.0, 0.0};
        ASSERT_DOUBLE_EQ(v.len(), 5.0);
    }

    {
        ivkg::vector_t<3> v{0.0, 0.0, 0.0};
        ASSERT_DOUBLE_EQ(v.len(), 0.0);
    }

    {
        ivkg::vector_t<3> v{16.0, 0.0, 0.0};
        ASSERT_DOUBLE_EQ(v.len(), 16.0);
    }
}

TEST(GeometryVectorTest, IsZero) {
    {
        ivkg::vector_t<3> v{3.0, 4.0, 0.0};
        ASSERT_FALSE(v.zero());
    }

    {
        ivkg::vector_t<3> v{0.0, 0.0, 0.0};
        ASSERT_TRUE(v.zero());
    }

    {
        ivkg::vector_t<3> v;
        ASSERT_FALSE(v.zero());
    }
}

TEST(GeometryVectorTest, Equal) {
    {
        ivkg::vector_t<3> v1{.0, 4.0, 0.0};
        ivkg::vector_t<3> v2{.0, 4.0, 0.0};
        ASSERT_TRUE(v1 == v2);
    }

    {
        ivkg::vector_t<3> v1{0.0, 4.0, 0.0};
        ivkg::vector_t<3> v2{0.0, 4.0, 0.0};
        ASSERT_TRUE(v1 == v2);
    }

    {
        ivkg::vector_t<3> v1{0.0, 4.0, 0.0};
        ivkg::vector_t<3> v2;
        ASSERT_FALSE(v1 == v2) << "1 invalid vector";
    }

    {
        ivkg::vector_t<3> v1{3.0, 4.0, 0.0};
        ivkg::point_t<3> p1{1.0, 0.0, 0.0};
        ivkg::point_t<3> p2{4.0, 4.0 , 0.0};
        ivkg::vector_t<3> v2(p2, p1);
        ASSERT_FALSE(v1 == v2) << "1 vector construct from points";
    }
}


TEST(GeometryVectorTest, IsParallel) {
    {
        ivkg::vector_t<3> v1{3.0, 4.0, 0.0};
        ivkg::vector_t<3> v2{6.0, 8.0, 0.0};
        ASSERT_TRUE(parallel(v1, v2));
    }

    {
        ivkg::vector_t<3> v1{3.0, 4.0, 0.0};
        ivkg::vector_t<3> v2{1.0, 8.0, 0.0};
        ASSERT_FALSE(parallel(v1, v2));
    }

    {
        ivkg::vector_t<3> v1{3.0, 4.0, 0.0};
        ivkg::vector_t<3> v2;
        ASSERT_FALSE(parallel(v1, v2)) << "1 vector invalid";
    }
}


TEST(GeometryVectorTest, Dot) {
    {
        ivkg::vector_t<3> a{1.0, 2.0, 3.0};
        ivkg::vector_t<3> b{0.0, 4.0, 3.0};
        ASSERT_DOUBLE_EQ(17.0, dot(a, b));
    }
}

TEST(GeometryVectorTest, Cross) {
    {
        ivkg::vector_t<3> a{1.0, 2.0, 3.0};
        ivkg::vector_t<3> b{-2.0, 0.0, 4.0};
        ivkg::vector_t<3> expected{8.0, -10.0, 4.0};
        ASSERT_EQ(expected, cross(a, b));
    }
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}