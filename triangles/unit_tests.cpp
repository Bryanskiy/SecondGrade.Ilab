#include <gtest/gtest.h>
#include <sstream>
#include <string>

#include "geometry.h"

/* ------------------------------------------------
                START POINT_TESTS
 -------------------------------------------------*/
TEST(GeometryPointTest, ValidTest) {
    {
        point3D_t x1;
        ASSERT_FALSE(x1.valid());
    }

    {
        point3D_t x(1.0, 2.0, 3.0);
        ASSERT_TRUE(x.valid());
    }
}

TEST(GeometryPointTest, CompareTest) {
    {
        point3D_t x1(1, 2, 3);
        point3D_t x2(1, 2, 3);
        ASSERT_TRUE(x1 == x2);
    }

    {
        point3D_t x1(1.0, 2.0, 3.0);
        point3D_t x2(0.0, 0.0, 0.0);
        ASSERT_FALSE(x1 == x2);
    }

    {
        point3D_t x1(1.0, 2.0, 3.0);
        point3D_t x2;
        ASSERT_FALSE(x1 == x2) << "x2 - invalid point";
    }
}

TEST(GeometryPointTest, IOtest) {
    {
        std::istringstream in("1.0 2.0 3.0");
        point3D_t p; in >> p;
        ASSERT_DOUBLE_EQ(1.0, p.x);
        ASSERT_DOUBLE_EQ(2.0, p.y);
        ASSERT_DOUBLE_EQ(3.0, p.z);
    }

    {
        std::ostringstream out;
        point3D_t p(1.1, 2.2, 3.3); out << p;
        std::string expected = "(1.1, 2.2, 3.3)";
        std::string actual = out.str();
        ASSERT_EQ(expected, actual);
    }
}

TEST(GeometryPointTest, DistanceTest) {
    {
        point3D_t x1(0.0, 0.0, 0.0);
        point3D_t x2(0.0, 0.0, 0.0);
        ASSERT_DOUBLE_EQ(distance(x1, x2), 0.0) << "zero distance";
    }

    {
        point3D_t x1(0.0, 0.0, 0.0);
        point3D_t x2(3.0, 0.0, 0.0);
        ASSERT_DOUBLE_EQ(distance(x1, x2), 3.0);
    }

    {
        point3D_t x1(0.0, 0.0, 0.0);
        point3D_t x2(3.0, 4.0, 0.0);
        ASSERT_DOUBLE_EQ(distance(x1, x2), 5.0);
    }
}
/* ------------------------------------------------
                END POINT_TESTS
 -------------------------------------------------*/



/* ------------------------------------------------
                START VECTOR_TESTS
 -------------------------------------------------*
*/
TEST(GeometryVectorTest, Arithmetic) {
    {
        vector3D_t a(1.0, 2.0, 3.0);
        vector3D_t b(0.0, 4.0, 3.0);
        vector3D_t expected(1.0, 6.0, 6.0);
        ASSERT_EQ(a + b, expected);
        ASSERT_EQ(a += b, expected);
    }

    {
        vector3D_t a(1.0, 2.0, 3.0);
        vector3D_t b(0.0, 4.0, 3.0);
        vector3D_t expected(1.0, -2.0, 0.0);
        ASSERT_EQ(a - b, expected);
        ASSERT_EQ(a -= b, expected);
    }

    {
        vector3D_t a(1.0, 2.0, 3.0);
        vector3D_t a_copy(1.0, 2.0, 3.0);
        vector3D_t expected(-1.0, -2.0, -3.0);
        ASSERT_EQ(-a, expected);
        ASSERT_EQ(a, a_copy) << "unary sub modify vector";
    }

    {
        vector3D_t a(1.0, 2.0, 3.0);
        double lambda = 4.0;
        vector3D_t expected(4.0, 8.0, 12.0);
        ASSERT_EQ(a * lambda, expected) << "mult by positive lambda";
        ASSERT_EQ(a *= lambda, expected) << "mult by positive lambda";
    }

    {
        vector3D_t a(1.0, 2.0, 0.0);
        double lambda = 0.0;
        vector3D_t expected(0.0, 0.0, 0.0);
        ASSERT_EQ(a * lambda, expected) << "mult by zero lambda";
        ASSERT_EQ(a *= lambda, expected) << "mult by zero lambda";
    }

    {
        vector3D_t a(1.0, 2.0, 0.0);
        double lambda = -1.0;
        vector3D_t expected(-1.0, -2.0, 0.0);
        ASSERT_EQ(a * lambda, expected) << "mult by negative lambda";
        ASSERT_EQ(a *= lambda, expected) << "mult by negative lambda";
    }
}

TEST(GeometryVectorTest, Valid) {
    {
        vector3D_t x;
        ASSERT_FALSE(x.valid()) << "default construct";
    }

    {
        point3D_t x1(1.0, 2.0, 3.0);
        point3D_t x2(1.0, 3.0, 5.0);
        vector3D_t x(x1, x2);
        ASSERT_TRUE(x.valid()) << "construct from 2 valid points";
    }

    {
        point3D_t x1;
        point3D_t x2(1.0, 3.0, 5.0);
        vector3D_t x(x1, x2);
        ASSERT_FALSE(x.valid()) << "construct from 1 invalid and 1 valid points";
    }

    {
        point3D_t x1;
        point3D_t x2;
        vector3D_t x(x1, x2);
        ASSERT_FALSE(x.valid()) << "construct from 2 invalid points";
    }

    {
        vector3D_t x(1.0, 2.0, 3.0);
        ASSERT_TRUE(x.valid()) << "construct doubles vector";
    }

}

TEST(GeometryVectorTest, Dot) {
    {
        vector3D_t a(1.0, 2.0, 3.0);
        vector3D_t b(0.0, 4.0, 3.0);
        ASSERT_DOUBLE_EQ(17.0, dot(a, b));
    }
}

TEST(GeometryVectorTest, Cross) {
    {
        vector3D_t a(1.0, 2.0, 3.0);
        vector3D_t b(-2.0, 0.0, 4.0);
        vector3D_t expected(8.0, -10.0, 4.0);
        ASSERT_EQ(expected, cross(a, b));
    }
}
/* ------------------------------------------------
                END VECTOR_TESTS
 -------------------------------------------------*
*/




/* ------------------------------------------------
                START TRIANGLE_TESTS
 -------------------------------------------------*/
TEST(GeometryTriangleTest, IOtest) {
    {
        std::istringstream in("1.0 0.0 0.0"
                              "0.0 0.0 0.0"
                              "0.0 2.0 0.0");
        triangle_t tr; in >> tr;
        point3D_t A(1.0, 0.0, 0.0);
        point3D_t B(0.0, 0.0, 0.0);
        point3D_t C(0.0, 2.0, 0.0);

        ASSERT_EQ(A, tr.A);
        ASSERT_EQ(B, tr.B);
        ASSERT_EQ(C, tr.C);
    }

    {
        std::ostringstream out;
        triangle_t p(point3D_t(1.0, 0.0, 0.0),
                     point3D_t(0.0, 0.0, 0.0),
                     point3D_t(0.0, 2.0, 0.0));
        out << p;
        std::string expected = "[(1, 0, 0) (0, 0, 0) (0, 2, 0)]";
        std::string actual = out.str();
        ASSERT_EQ(expected, actual);
    }
}
/* ------------------------------------------------
                END TRIANGLE_TESTS
 -------------------------------------------------*/



/* ------------------------------------------------
                START PLANE_TESTS
 -------------------------------------------------*/
TEST(GeometryPlaneTest, ConstructFromPoints) {
    {
        point3D_t x1(1.0, 2.0, 3.0);
        point3D_t x2(1.0, 2.0, 4.0);
        point3D_t x3(4.0, 2.0, -1.0);

        plane_t p(x1, x2, x3);
        ASSERT_DOUBLE_EQ(0.0, p.a);
        ASSERT_DOUBLE_EQ(3.0, p.b);
        ASSERT_DOUBLE_EQ(0.0, p.c);
        ASSERT_DOUBLE_EQ(-6.0, p.d);
    }

    {
        point3D_t x1(-3.0, 2.0, -1.0);
        point3D_t x2(-1.0, 2.0, 4.0);
        point3D_t x3(3.0, 3.0, -1.0);

        plane_t p(x1, x2, x3);
        ASSERT_DOUBLE_EQ(-5.0, p.a);
        ASSERT_DOUBLE_EQ(30.0, p.b);
        ASSERT_DOUBLE_EQ(2.0, p.c);
        ASSERT_DOUBLE_EQ(-73.0, p.d);
    }

}

TEST(GeometryPlaneTest, IsParallel) {
    {
        plane_t p1(1.0, 1.0, 1.0, 0.0);
        plane_t p2(2.0, 2.0, 2.0, 5.0);
        ASSERT_TRUE(is_parallel(p1, p2));
    }

    {
        plane_t p1(1.0, 1.0, 1.0, 0.0);
        plane_t p2(3.0, 2.0, 2.0, 5.0);
        ASSERT_FALSE(is_parallel(p1, p2));
    }

}

/* ------------------------------------------------
                END PLANE_TESTS
 -------------------------------------------------*/



/* ------------------------------------------------
                START INTERSECTION_TESTS
 ------------------------------------------------*/

TEST(GeometryIntersectionTest, LineAndTriangle2D) {
    {
        line_t line(point3D_t(1.0, 0.0, 0.0), point3D_t(0.0, 1.0, 0.0));
        triangle_t tr(point3D_t(1.0, 0.0, 0.0), point3D_t(2.0, 0.0, 0.0), point3D_t(1.0, 1.0, 0.0));
        point3D_t p = intersection_line_triangle_2D(line, tr);
        ASSERT_EQ(1.0, p.x);
        ASSERT_EQ(0.0, p.y);
        ASSERT_EQ(0.0, p.z);
    }
}

/* ------------------------------------------------
                END INTERSECTION_METHODS
 ------------------------------------------------*/
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}