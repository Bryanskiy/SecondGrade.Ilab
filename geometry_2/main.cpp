#include <iostream>
#include <stdexcept>

#include "Vkdriver.hpp"

int main() {
    vkdriver::Vkdriver driver;

    try {
        driver.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

