#pragma once

#include <FlexLexer.h>
#include <string>

#include "../node_interface/Inode.hpp"
#include "../parser/compiler.tab.hh"


namespace yy {
    class driver_t final {
    public:
        driver_t();
        ~driver_t();

        parser::token_type yylex(parser::semantic_type* yylval);

        bool parse();
    private:
        yyFlexLexer* plexer_;
    };
}