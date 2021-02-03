#include "driver.hpp"

Inode::Iscope_t* current_scope;

namespace yy {

driver_t::driver_t() {
    plexer_ = new yyFlexLexer;
    current_scope = Inode::make_scope();
}

parser::token_type driver_t::yylex(parser::semantic_type* yylval) {
    parser::token_type token = static_cast<parser::token_type>(plexer_->yylex());
    if(token == yy::parser::token_type::NAME) {
        std::string name(plexer_->YYText());
        parser::semantic_type tmp;
        tmp.as<std::string>() = name;
        yylval->swap<std::string>(tmp);
    } else if(token == yy::parser::token_type::INTEGER) {
        yylval->as<int>() = std::atoi(plexer_->YYText());
    }

    return token;
}

bool driver_t::parse() {
    parser parser(this);
    bool res = parser.parse();
    return !res;
}

driver_t::~driver_t() {
    delete plexer_;
}

}