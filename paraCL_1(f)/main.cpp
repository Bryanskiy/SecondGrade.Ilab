#include "driver/driver.hpp"

int main(int argc, char** argv) {
     if(argc < 2) {
         std::cerr << "Expected input file" << std::endl;
         return 0;
    } 

    try {
        yy::driver_t driver(argv[1]);
        driver.parse();
    } catch(std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }    
}