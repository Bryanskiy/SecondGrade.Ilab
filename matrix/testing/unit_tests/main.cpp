#include <gtest/gtest.h>

#include "constructors.h"
#include "selectors.h"
#include "operators.h"
#include "useful_methods.h"

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}