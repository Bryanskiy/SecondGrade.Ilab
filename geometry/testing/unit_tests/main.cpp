#include <gtest/gtest.h>
#include <sstream>
#include <string>

#include "../../include/geometry.h"
#include "line_tests.h"
#include "point_tests.h"
#include "vector_tests.h"
#include "distance.h"
#include "intersection.h"

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}