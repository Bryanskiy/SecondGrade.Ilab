#include "driver/driver.hpp"

int main(int argc, char** argv) {
     if(argc < 2) {
         std::cerr << "Expected input file" << std::endl;
         return 0;
    } 

    yy::driver_t driver(argv[1]);
    driver.parse();
}