#pragma once 

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif
#include <iostream>
#include "compiler.tab.hh"

class mylexer_t : public yyFlexLexer {
private:
    yy::location current_location_;
    std::size_t last_line_;
public:

    yy::location get_current_location() const {return current_location_;}
    std::size_t get_current_line() const {return last_line_;}

    void update_location() {
        std::size_t current_line = lineno();
        current_location_.lines(current_line - last_line_);
        current_location_.step();
        current_location_.columns(YYLeng());
        last_line_ = current_line;
    }

    int yylex() override;

    virtual ~mylexer_t() = default;
};