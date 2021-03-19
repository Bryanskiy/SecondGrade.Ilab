#include "driver.hpp"

Inode::scope_t* current_scope;
yy::location current_location;

namespace yy {

driver_t::driver_t() {
    plexer_ = new mylexer_t;
    current_scope = Inode::make_scope();
}

driver_t::driver_t(const char* file_name) : file_name_(file_name) {
    plexer_ = new mylexer_t;
    current_scope = Inode::make_scope();

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
    std::cerr << color_t::set_red() << "-----------------[Syntax error]---------------------" << color_t::reset() << std::endl <<
    color_t::set_cyan() << "LOCATION: " << color_t::reset() << file_name_ <<  " : " << 
    plexer_->get_current_line() << " line" << std::endl;

    enum { TOKENMAX = 26 };
    parser::symbol_kind_type expected_tokens[TOKENMAX];
    int expected_tokens_count = ctx.expected_tokens(expected_tokens, TOKENMAX);

    std::cerr << color_t::set_cyan() << "Expected: " << color_t::reset();
    for (int i = 0; i < expected_tokens_count; i++) {
        if (i != 0) { 
            std::cerr << " or "; 
        }

        std::cerr << "<" << parser::symbol_name(expected_tokens[i]) << ">";
    }
    std::cerr << color_t::set_cyan() << " before " << color_t::reset() << "<" << parser::symbol_name(ctx.token()) << ">" << std::endl;
    std::cerr << color_t::set_cyan() << "LINE: " << color_t::reset() << code_lines_[plexer_->get_current_line() - 1] << std::endl;
    std::cerr << "      " << color_t::set_red();

    yy::location loc = ctx.location();
    for (size_t i = 0; i < loc.end.column; i++) {
        if (i == ctx.lookahead().location.begin.line - 1) {
            std::cerr << '^';
        }
        else {
            std::cerr << '~';
        }
    }
    std::cerr << color_t::reset();
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