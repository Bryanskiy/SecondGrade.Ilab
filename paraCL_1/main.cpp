#include "driver/driver.hpp"

int yyFlexLexer::yywrap() {
    return 1;
}

int main(int argc, char** argv) {
    // if(argc < 2) {
    //     std::cerr << "Expected input file" << std::endl;
    //     return 0;
    // } 

    yy::driver_t driver;
    driver.parse();
}