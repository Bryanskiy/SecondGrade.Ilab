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

TEST(GeometryVectorTest, Len) {
    {
        vector3D_t v(3.0, 4.0, 0.0);
        ASSERT_DOUBLE_EQ(v.len(), 5.0);
    }

    {
        vector3D_t v(0.0, 0.0, 0.0);
        ASSERT_DOUBLE_EQ(v.len(), 0.0);
    }

    {
        vector3D_t v(16.0, 0.0, 0.0);
        ASSERT_DOUBLE_EQ(v.len(), 16.0);
    }
}

TEST(GeometryVectorTest, IsZero) {
    {
        vector3D_t v(3.0, 4.0, 0.0);
        ASSERT_FALSE(v.is_zero());
    }

    {
        vector3D_t v(0.0, 0.0, 0.0);
        ASSERT_TRUE(v.is_zero());
    }

    {
        vector3D_t v;
        ASSERT_FALSE(v.is_zero());
    }
}

TEST(GeometryVectorTest, Equal) {
    {
        vector3D_t v1(3.0, 4.0, 0.0);
        vector3D_t v2(3.0, 4.0, 0.0);
        ASSERT_TRUE(v1 == v2);
    }

    {
        vector3D_t v1(3.0, 4.0, 0.0);
        vector3D_t v2(1.0, 4.0, 0.0);
        ASSERT_FALSE(v1 == v2);
    }

    {
        vector3D_t v1(3.0, 4.0, 0.0);
        vector3D_t v2;
        ASSERT_FALSE(v1 == v2) << "1 invalid vector";
    }

    {
        vector3D_t v1(3.0, 4.0, 0.0);
        point3D_t p1(1.0, 0.0, 0.0);
        point3D_t p2(4.0, 4.0 , 0.0);
        vector3D_t v2(p2, p1);
        ASSERT_FALSE(v1 == v2) << "1 vector construct from points";
    }
}

TEST(GeometryVectorTest, IsParallel) {
    {
        vector3D_t v1(3.0, 4.0, 0.0);
        vector3D_t v2(6.0, 8.0, 0.0);
        ASSERT_TRUE(is_parallel(v1, v2));
    }

    {
        vector3D_t v1(3.0, 4.0, 0.0);
        vector3D_t v2(1.0, 8.0, 0.0);
        ASSERT_FALSE(is_parallel(v1, v2));
    }

    {
        vector3D_t v1(3.0, 4.0, 0.0);
        vector3D_t v2;
        ASSERT_FALSE(is_parallel(v1, v2)) << "1 vector invalid";
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
                START LINE_TESTS
 -------------------------------------------------*/
TEST(GeometryLineTest, Include) {
    {
        point3D_t p1(0.0, 0.0, 0.0);
        point3D_t p2(1.0, 1.0, 0.0);
        line_t line(p1, p2);
        double t = -1.0;
        point3D_t point(0.0, 0.0, 0.0);
        t = line.intersect(point);
        ASSERT_DOUBLE_EQ(0.0, t);
    }

    {
        point3D_t p1(0.0, 0.0, 0.0);
        point3D_t p2(1.0, 1.0, 0.0);
        line_t line(p1, p2);
        point3D_t point(-1.0, 0.0, 0.0);
        ASSERT_TRUE(std::isnan(line.intersect(point)));
    }
}

// todo : check if line construct from 2 equal points
TEST(GeometryLineTest, Valid) {
    {
        line_t line;
        ASSERT_FALSE(line.valid());
    }

    {
        point3D_t p1(0.0, 1.0, 2.0);
        point3D_t p2;
        line_t line(p1, p2);
        ASSERT_FALSE(line.valid()) << "construct from 2 points, 1 point invalid";
    }

    {
        point3D_t p1(0.0, 1.0, 2.0);
        point3D_t p2(1.0, 3.0, 4.0);
        line_t line(p1, p2);
        ASSERT_TRUE(line.valid());
    }
}

TEST(GeometryLineTest, Isparallel) {
    {
        point3D_t p1(0.0, 0.0, 0.0);
        point3D_t p2(1.0, 2.0, 0.0);
        line_t l1(p1, p2);

        point3D_t p3(0.0, 0.0, 0.0);
        point3D_t p4(2.0, 4.0, 0.0);
        line_t l2(p3, p4);

        ASSERT_TRUE(is_parallel(l1, l2));
    }

    {
        point3D_t p1(0.0, 0.0, 0.0);
        point3D_t p2(1.0, 2.0, 0.0);
        line_t l1(p1, p2);

        point3D_t p3(0.0, 0.0, 0.0);
        point3D_t p4(2.0, 5.0, 0.0);
        line_t l2(p3, p4);

        ASSERT_FALSE(is_parallel(l1, l2));
    }

    {
        point3D_t p1(0.0, 0.0, 0.0);
        point3D_t p2(1.0, 2.0, 0.0);
        line_t l1(p1, p2);

        line_t l2;

        ASSERT_FALSE(is_parallel(l1, l2)) << "one line is invalid";
    }
}
/* ------------------------------------------------
                END LINE_TESTS
 -------------------------------------------------*/




/* ------------------------------------------------
                START SEGMENT_TESTS
 -------------------------------------------------*/
TEST(GeometrySegmentTest, Include) {
    {
        point3D_t p1(0.0, 0.0, 0.0);
        point3D_t p2(3.0, 4.0, 5.0);
        segment_t s(p1, p2);

        point3D_t point(0.0, 0.0, 0.0);
        double t = s.intersect(point);
        ASSERT_DOUBLE_EQ(0.0, t);
    }

    {
        point3D_t p1(0.0, 0.0, 0.0);
        point3D_t p2(3.0, 4.0, 5.0);
        segment_t s(p1, p2);

        point3D_t point(-1.0, 0.0, 0.0);
        double t = s.intersect(point);
        ASSERT_TRUE(std::isnan(t));
    }
}
/* ------------------------------------------------
                END SEGMENT_TESTS
 -------------------------------------------------*/




/* ------------------------------------------------
                START TRIANGLE_TESTS
 -------------------------------------------------*/

// todo: 3 point on 1 line
TEST(GeometryTriangleTest, Valid) {
    {
        triangle_t x;
        ASSERT_FALSE(x.valid());
    }

    {
        point3D_t a(0.0, 0.0, 0.0);
        point3D_t b(2.0, 5.0, 1.0);
        point3D_t c(2.0, 5.0, 2.0);
        triangle_t x(a, b, c);
        ASSERT_TRUE(x.valid());
    }
}

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

TEST(GeometryPlaneTest, Equal) {
    {
        plane_t p1(1.0, 2.0, 0.0, 3.0);
        plane_t p2(2.0, 4.0, 0.0, 6.0);
        ASSERT_TRUE(p1 == p2);
    }

    {
        plane_t p1(1.0, 2.0, 0.0, 2.0);
        plane_t p2(2.0, 4.0, 0.0, 6.0);
        ASSERT_FALSE(p1 == p2);
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
//todo : if lines are equal
TEST(GeometryIntersectionTest, Intersection2Lines) {
    {
        point3D_t p1(0.0, 0.0, 0.0);
        point3D_t p2(1.0, 1.0, 0.0);
        line_t l1(p1, p2);

        point3D_t p3(-1.0, 1.0, 0.0);
        line_t l2(p1, p3);

        point3D_t intersection = l1.intersect(l2);
        ASSERT_TRUE(intersection == p1);
    }

    {
        point3D_t p1(0.0, 0.0, 0.0);
        point3D_t p2(1.0, 1.0, 0.0);
        line_t l1(p1, p2);

        point3D_t p3(-1.0, 1.0, 5.0);
        point3D_t p4(1.0, 2.0, 5.0);
        line_t l2(p3, p4);

        point3D_t intersection = l1.intersect(l2);
        ASSERT_FALSE(intersection.valid());
    }
}

TEST(GeometryIntersectionTest, Intersection2Segments) {
    {
        point3D_t p1(0.0, 0.0, 0.0);
        point3D_t p2(1.0, 1.0, 0.0);
        segment_t l1(p1, p2);

        point3D_t p3(-1.0, 1.0, 0.0);
        segment_t l2(p1, p3);

        point3D_t intersection = intersection_of_2segments(l1, l2);
        ASSERT_TRUE(intersection == p1);
    }

    {
        point3D_t p1(0.0, 0.0, 0.0);
        point3D_t p2(1.0, 1.0, 0.0);
        segment_t l1(p1, p2);

        point3D_t p3(-1.0, 1.0, 5.0);
        point3D_t p4(1.0, 2.0, 5.0);
        segment_t l2(p3, p4);

        point3D_t intersection = intersection_of_2segments(l1, l2);
        ASSERT_FALSE(intersection.valid());
    }
}

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

                END INTERSECTION_METHODS
 ------------------------------------------------*/
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}