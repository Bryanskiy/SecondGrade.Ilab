#include <iostream>
#include <vector>
#include <gtest/gtest.h>

#include "../dirichlet/dirichlet.hpp"
#include "../cl_framework/cl_linalg.hpp"

TEST(ClLinalg, Add) {
    {
        clf::cl_fvector_t vec1({1.0, 1.0, 1.0});
        clf::cl_fvector_t vec2({1.0, 2.0, 3.0});
        clf::cl_fvector_t vec3 = vec1 + vec2;
        
        clf::cl_fvector_t ans({2.0, 3.0, 4.0}); 
        ASSERT_TRUE(ans == vec3) << "Equal vector sizes";
    }

    {
        clf::cl_fvector_t vec1({1.0, 1.0, 1.0});
        clf::cl_fvector_t vec2({1.0, 2.0, 3.0, 4.0});
        clf::cl_fvector_t vec3 = vec1 + vec2;
        
        clf::cl_fvector_t ans({2.0, 3.0, 4.0, 4.0}); 
        ASSERT_TRUE(ans == vec3) << "v1.size() < v2.size()";
    }

    {
        clf::cl_fvector_t vec1({1.0, 1.0, 1.0, 5.0, 6.0});
        clf::cl_fvector_t vec2({1.0, 2.0, 3.0, 4.0});
        clf::cl_fvector_t vec3 = vec1 + vec2;
        
        clf::cl_fvector_t ans({2.0, 3.0, 4.0, 9.0, 6.0}); 
        ASSERT_TRUE(ans == vec3) << "v1.size() > v2.size()";
    }
}

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}