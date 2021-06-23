#include "CLFramework/CLFramework.hpp"

int main() {
    std::vector<float> v{1.0, 2.0, 3.0};
    clfm::CLfVector clv(v.begin(), v.end());

    std::cout << clv.size();
}