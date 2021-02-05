#pragma once

#include <FlexLexer.h>
#include <string>

#include "../node_interface/Inode.hpp"
#include "../parser/compiler.tab.hh"
#include "../parser/mylexer.hpp"


namespace yy {
    class driver_t final {
    public:
        driver_t();
        driver_t(const char* file_name);
        ~driver_t();

        parser::token_type yylex(parser::location_type* l, parser::semantic_type* yylval);
        void report_syntax_error(parser::context const& ctx) const;
        void set_source_stream();
        bool parse();
    private:
        mylexer_t* plexer_;
    };
}

struct color_t {
static const char* set_black() {return "\e[0;30m";}    
static const char* set_red() {return "\e[0;31m";}
static const char* set_green() {return "\e[0;92m";}
static const char* set_cyan() {return "\e[0;96m";}
static const char* reset() {return "\e[0m";}
};