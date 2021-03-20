#pragma once

#include <FlexLexer.h>
#include <string>
#include <fstream>
#include <sstream>

#include "../node_interface/node.hpp"
#include "../parser/compiler.tab.hh"
#include "../parser/mylexer.hpp"


namespace yy {
    class driver_t final {
    public:

        enum error_type {
            RUNTIME_UNDEFINED_NAME,
        };

        driver_t();
        driver_t(const char* file_name);
        ~driver_t();

        parser::token_type yylex(parser::location_type* l, parser::semantic_type* yylval);
        void report_syntax_error(parser::context const& ctx) const;
        void report_runtime_error(const parser::location_type& location, error_type err) const;
        bool parse();
    private:
        const std::string file_name_;
        std::vector<std::string> code_lines_;
        std::ifstream file_;

        mylexer_t* plexer_;

        void report_error_position(std::ostream& stream, const parser::location_type& location) const;
    };
}

struct color_t {
    static const char* set_black() {return "\e[0;30m";}    
    static const char* set_red() {return "\e[0;31m";}
    static const char* set_green() {return "\e[0;92m";}
    static const char* set_cyan() {return "\e[0;96m";}
    static const char* reset() {return "\e[0m";}
};