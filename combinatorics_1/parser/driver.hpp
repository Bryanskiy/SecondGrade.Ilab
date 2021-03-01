#pragma once

#include <fstream>

#include "mylexer.hpp"

namespace yy {

struct edge_t {
    std::size_t v1, v2, w;
};

struct color_t {
static const char* set_black() {return "\e[0;30m";}    
static const char* set_red() {return "\e[0;31m";}
static const char* set_green() {return "\e[0;92m";}
static const char* set_cyan() {return "\e[0;96m";}
static const char* reset() {return "\e[0m";}
};

class driver_t {

public:
    driver_t() : plexer_ (new mylexer_t) {}
    driver_t(const char* file_name) : file_name_(file_name) {
        plexer_ = new mylexer_t;

        file_.open(file_name_);
        plexer_->switch_streams(file_, std::cout);
    }
    ~driver_t() {
        delete plexer_;
        file_.close();
    }

    void push(std::size_t v1, std::size_t v2, std::size_t w) {
        edges_.push_back({v1, v2, w});
    }

    std::vector<edge_t> get_edges() const {
        return edges_;
    }

    parser::token_type yylex(parser::location_type* l, parser::semantic_type* yylval) {
        parser::token_type token = static_cast<parser::token_type>(plexer_->yylex());
        if(token == yy::parser::token_type::INTEGER) {
            yylval->as<int>() = std::atoi(plexer_->YYText());
        }    

        /* lexer update current location after plexer_->yylex() and token match */
        *l = plexer_->get_current_location();

        return token;
    }

    void report_syntax_error(parser::context const& ctx) const {
        int res = 0;
        std::cerr << color_t::set_red() << "-----------------[Syntax error]---------------------" << color_t::reset() << std::endl <<
        color_t::set_cyan() << "LINE: " << plexer_->get_current_line() << std::endl;

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
    }
    
    bool parse() {
        parser parser(this);
        bool res = parser.parse();
        return !res;
    }

private:

    std::ifstream file_;
    const char* file_name_ = nullptr;
    mylexer_t* plexer_;
    std::vector<edge_t> edges_;
};


} /* namespace yy */