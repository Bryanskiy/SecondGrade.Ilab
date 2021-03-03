#pragma once

#include <fstream>
#include <algorithm>

#include "mylexer.hpp"
#include "../matrix/matrix.hpp"

struct color_t {
static const char* set_black() {return "\e[0;30m";}    
static const char* set_red() {return "\e[0;31m";}
static const char* set_green() {return "\e[0;92m";}
static const char* set_cyan() {return "\e[0;96m";}
static const char* reset() {return "\e[0m";}
};

namespace yy {

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

    void push(std::size_t v1, std::size_t v2, double resistance, double voltage) {
        std::size_t new_cols = eds_matrix_.get_cols_number() + 1;
        std::size_t new_rows = std::max(eds_matrix_.get_rows_number() , std::max(v1, v2) + 1);

        eds_matrix_.resize(new_rows, new_cols);
        resistance_matrix_.resize(new_rows, new_cols);
        edges_matrix_.resize(new_rows, new_cols);

        eds_matrix_[v1][eds_matrix_.get_cols_number() - 1] = voltage;
        eds_matrix_[v2][eds_matrix_.get_cols_number() - 1] = voltage;

        resistance_matrix_[v1][resistance_matrix_.get_cols_number() - 1] = resistance;
        resistance_matrix_[v2][resistance_matrix_.get_cols_number() - 1] = resistance;

        edges_matrix_[v1][edges_matrix_.get_cols_number() - 1] = 1;
        edges_matrix_[v2][edges_matrix_.get_cols_number() - 1] = 2;
    }

    matrix::matrix_t<double> get_resistance_matrix() const {
        return resistance_matrix_;
    }

    matrix::matrix_t<double> get_eds_matrix() const {
        return eds_matrix_;
    }

    matrix::matrix_t<int> get_edges_matrix() const {
        return edges_matrix_;
    }

    parser::token_type yylex(parser::location_type* l, parser::semantic_type* yylval) {
        parser::token_type token = static_cast<parser::token_type>(plexer_->yylex());
        if(token == yy::parser::token_type::INTEGER) {
            yylval->as<int>() = std::atoi(plexer_->YYText());
        } else if(token == yy::parser::token_type::DOUBLE) {
            yylval->as<float>() = std::stof(plexer_->YYText());
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
    matrix::matrix_t<double> eds_matrix_;
    matrix::matrix_t<double> resistance_matrix_;
    matrix::matrix_t<int>    edges_matrix_;
};


} /* namespace yy */