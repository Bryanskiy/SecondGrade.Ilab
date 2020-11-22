#pragma once

#include <FlexLexer.h>
#include <string>

#include "Inode.hpp"
#include "compiler.tab.hh"


namespace yy {
    class driver_t final {
    public:
        driver_t(FlexLexer* plexer) : plexer_(plexer) {}

        parser::token_type yylex(parser::semantic_type* yylval) {
            parser::token_type tt = static_cast<parser::token_type>(plexer_->yylex());
            if (tt == yy::parser::token_type::INTEGER)
                yylval->integer_ = std::stoi(plexer_->YYText());

            if(tt == yy::parser::token_type::NAME) {
                yylval->name_ = std::string(plexer_->YYText());
            }
            
            return tt;
        }

        bool parse() {
            parser parser(this);
            bool res = parser.parse();
            return !res;
        }
    private:
        FlexLexer* plexer_;
    };
}