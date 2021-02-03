#include "driver/driver.hpp"

int yyFlexLexer::yywrap() {
  return 1;
}

int main() {
    yy::driver_t driver;
    driver.parse();
}