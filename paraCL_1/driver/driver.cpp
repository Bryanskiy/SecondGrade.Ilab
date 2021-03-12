#include "driver.hpp"

node::scope_t* current_scope = nullptr;

namespace yy {

driver_t::driver_t() {
    plexer_ = new yyFlexLexer;
    current_scope = new node::scope_t(nullptr);
}

driver_t::driver_t(const char* file_name) : file_name_(file_name) {
    plexer_ = new yyFlexLexer;
    current_scope = new node::scope_t(nullptr);

    file_.open(file_name_);
    std::ifstream tmp(file_name_);
    while(tmp) {
        std::string str;
        std::getline(tmp, str);
        code_lines_.push_back(str);
    }

    plexer_->switch_streams(file_, std::cout);
}

parser::token_type driver_t::yylex(parser::location_type* l, parser::semantic_type* yylval) {
    parser::token_type token = static_cast<parser::token_type>(plexer_->yylex());
    if(token == yy::parser::token_type::VARIABLE) {
        std::string name(plexer_->YYText());
        parser::semantic_type tmp;
        tmp.as<std::string>() = name;
        yylval->swap<std::string>(tmp);
    } else if(token == yy::parser::token_type::INTEGER) {
        yylval->as<int>() = std::atoi(plexer_->YYText());
    }

    /* lexer update current location after plexer_->yylex() and token match */

    return token;
}

void driver_t::report_syntax_error(parser::context const& ctx) const {

}

bool driver_t::parse() {
    parser parser(this);
    bool res = parser.parse();
    return !res;
}

driver_t::~driver_t() {
    delete plexer_;
    delete current_scope;
}

}