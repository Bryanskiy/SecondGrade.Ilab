#include "driver.hpp"

Inode::Iscope_t* current_scope;
yy::location current_location;

namespace yy {

driver_t::driver_t() {
    plexer_ = new mylexer_t;
    current_scope = Inode::make_scope();
}

driver_t::driver_t(const char* file_name) : driver_t() {
    
}

parser::token_type driver_t::yylex(parser::location_type* l, parser::semantic_type* yylval) {
    parser::token_type token = static_cast<parser::token_type>(plexer_->yylex());
    if(token == yy::parser::token_type::NAME) {
        std::string name(plexer_->YYText());
        parser::semantic_type tmp;
        tmp.as<std::string>() = name;
        yylval->swap<std::string>(tmp);
    } else if(token == yy::parser::token_type::INTEGER) {
        yylval->as<int>() = std::atoi(plexer_->YYText());
    }

    /* lexer update current location after plexer_->yylex() and token match */
    *l = plexer_->get_current_location();

    return token;
}

void driver_t::report_syntax_error(parser::context const& ctx) const {
    int res = 0;
    std::cerr << color_t::set_red() << "Syntax error: " << color_t::reset() << plexer_->get_current_line() << ctx.location ();

    parser::symbol_kind_type lookahead = ctx.token ();
    if (lookahead != parser::symbol_kind::S_YYEMPTY)
      std::cerr << " before " << parser::symbol_name (lookahead);
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