#include <iostream>
#include <vector>
#include <gtest/gtest.h>

#include "../CLFramework/CLFramework.hpp"

TEST(ClLinalg, Add) {
    {
        clfm::CLfVector vec1({1.0, 1.0, 1.0});
        clfm::CLfVector vec2({1.0, 2.0, 3.0});
        clfm::CLfVector vec3 = vec1 + vec2;
        
        clfm::CLfVector ans({2.0, 3.0, 4.0}); 
        ASSERT_TRUE(ans == vec3) << "Equal vector sizes";
    }

    {
        clfm::CLfVector vec1({1.0, 1.0, 1.0});
        clfm::CLfVector vec2({1.0, 2.0, 3.0, 4.0});
        clfm::CLfVector vec3 = vec1 + vec2;
        
        clfm::CLfVector ans({2.0, 3.0, 4.0, 4.0}); 
        ASSERT_TRUE(ans == vec3) << "v1.size() < v2.size()";
    }

    {
        clfm::CLfVector vec1({1.0, 1.0, 1.0, 5.0, 6.0});
        clfm::CLfVector vec2({1.0, 2.0, 3.0, 4.0});
        clfm::CLfVector vec3 = vec1 + vec2;
        
        clfm::CLfVector ans({2.0, 3.0, 4.0, 9.0, 6.0}); 
        ASSERT_TRUE(ans == vec3) << "v1.size() > v2.size()";
    }
}

TEST(ClLinalg, Sub) {
    {
        clfm::CLfVector vec1({1.0, 1.0, 1.0});
        clfm::CLfVector vec2({1.0, 2.0, 3.0});
        clfm::CLfVector vec3 = vec1 - vec2;
        
        clfm::CLfVector ans({0.0, -1.0, -2.0}); 
        ASSERT_TRUE(ans == vec3) << "Equal vector sizes";
    }

    {
        clfm::CLfVector vec1({1.0, 1.0, 1.0});
        clfm::CLfVector vec2({1.0, 2.0, 3.0, 4.0});
        clfm::CLfVector vec3 = vec1 - vec2;
        
        clfm::CLfVector ans({0.0, -1.0, -2.0, -4.0}); 
        ASSERT_TRUE(ans == vec3) << "v1.size() < v2.size()";
    }

    {
        clfm::CLfVector vec1({1.0, 1.0, 1.0, 5.0, 6.0});
        clfm::CLfVector vec2({1.0, 2.0, 3.0, 4.0});
        clfm::CLfVector vec3 = vec1 - vec2;
        
        clfm::CLfVector ans({0.0, -1.0, -2.0, 1.0, 6.0}); 
        ASSERT_TRUE(ans == vec3) << "v1.size() > v2.size()";
    }
}

TEST(ClLinalg, ScalarMult) {
    {
        clfm::CLfVector vec1({1.0, 1.0, 1.0});
        vec1 *= 5.0;

        clfm::CLfVector ans({5.0, 5.0, 5.0});
        ASSERT_TRUE(vec1 == ans);
    }
}

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}