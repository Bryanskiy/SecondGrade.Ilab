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

TEST(ClLinalg, Sub) {
    {
        clf::cl_fvector_t vec1({1.0, 1.0, 1.0});
        clf::cl_fvector_t vec2({1.0, 2.0, 3.0});
        clf::cl_fvector_t vec3 = vec1 - vec2;
        
        clf::cl_fvector_t ans({0.0, -1.0, -2.0}); 
        ASSERT_TRUE(ans == vec3) << "Equal vector sizes";
    }

    {
        clf::cl_fvector_t vec1({1.0, 1.0, 1.0});
        clf::cl_fvector_t vec2({1.0, 2.0, 3.0, 4.0});
        clf::cl_fvector_t vec3 = vec1 - vec2;
        
        clf::cl_fvector_t ans({0.0, -1.0, -2.0, -4.0}); 
        ASSERT_TRUE(ans == vec3) << "v1.size() < v2.size()";
    }

    {
        clf::cl_fvector_t vec1({1.0, 1.0, 1.0, 5.0, 6.0});
        clf::cl_fvector_t vec2({1.0, 2.0, 3.0, 4.0});
        clf::cl_fvector_t vec3 = vec1 - vec2;
        
        clf::cl_fvector_t ans({0.0, -1.0, -2.0, 1.0, 6.0}); 
        ASSERT_TRUE(ans == vec3) << "v1.size() > v2.size()";
    }
}

TEST(ClLinalg, ScalarMult) {
    {
        clf::cl_fvector_t vec1({1.0, 1.0, 1.0});
        vec1.scalar_mult(5.0);

        clf::cl_fvector_t ans({5.0, 5.0, 5.0});
        ASSERT_TRUE(vec1 == ans);
    }
}

TEST(ClLinalg, Negate) {
    {
        clf::cl_fvector_t vec1({1.0, 1.0, 1.0});
        vec1.negate();

        clf::cl_fvector_t ans({-1.0, -1.0, -1.0});
        ASSERT_TRUE(vec1 == ans);
    }

    {
        clf::cl_fvector_t vec1({0.0, 0.0, 0.0});
        vec1.negate();

        clf::cl_fvector_t ans({0.0, 0.0, 0.0});
        ASSERT_TRUE(vec1 == ans);
    }
}

TEST(ClLinalg, ScalarMult2) {
    {
        matrix::matrix_t<float> m = { {1.0, 2.0, 3.0}, 
                                      {4.0, 5.0, 6.0}, 
                                      {7.0, 8.0, 9.0} };

        clf::cl_bandet_sparce_fmatrix_t bandet_m(m);
        clf::cl_fvector_t vec({1.0, 1.0, 1.0});
        clf::cl_fvector_t expected = bandet_m.vector_mult(vec);

        clf::cl_fvector_t ans({12.0, 15.0, 18.0});

        ASSERT_TRUE(expected == ans);
    }
}

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}