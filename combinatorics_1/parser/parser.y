%language "c++"
%skeleton "lalr1.cc"

%define api.value.type variant
%define parse.error custom
%param {driver_t* driver}

%locations

%code requires {
namespace yy { class driver_t; }
}

%code {
#include "driver.hpp"
namespace yy {parser::token_type yylex(parser::semantic_type* yylval, parser::location_type* l, driver_t* driver);}
}

%token <size_t>      INTEGER

%token EDGE          "--"
       COMMA         ","
       LBREAK        "\\n"
       END_OF_FILE   0

%nterm <size_t>      edge
%nterm <size_t>      vertex

%%

program:  line program                                          {}
        |   /* empty */ 	                                    {}

line:     expr LBREAK                                           {}
        |  expr END_OF_FILE                                     {}
        | LBREAK                                                {}   

expr :    vertex EDGE vertex COMMA edge                         {
                                                                    driver->push($1, $3, $5);
                                                                }

vertex:   INTEGER                                               {$$ = $1;}

edge:     INTEGER                                               {$$ = $1;}


%%

namespace yy {

    parser::token_type yylex (parser::semantic_type* yylval, parser::location_type* l, driver_t* driver) {
		return driver->yylex (l, yylval);
	}

	void parser::error (const parser::location_type& l, const std::string& msg) {
		std::cout << msg << " in line: " << l.begin.line << std::endl;
	}

	void parser::report_syntax_error(parser::context const& ctx) const {
		driver->report_syntax_error(ctx);
	}
} /* namespace yy */