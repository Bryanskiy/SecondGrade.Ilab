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

%token <size_t>      VERTEX

/* wtf with double ??? */
       <float>       DOUBLE

%token EDGE          "--"
       COMMA         ","
       SCOLON        ";"
       V             "V"
       LBREAK        "\\n"
       END_OF_FILE   0

%nterm <float>       voltage
%nterm <float>       resist
%nterm <size_t>      vertex

%%

program:  line program                                          {}
        |   /* empty */ 	                                    {}

line:     expr LBREAK                                           {}
        |  expr END_OF_FILE                                     {}
        | LBREAK                                                {}   

expr :   vertex EDGE vertex COMMA resist SCOLON voltage         {
                                                                    driver->push($1, $3, $5, $7);
                                                                }

vertex: VERTEX                                                  {$$ = $1;}     
resist: DOUBLE                                                  {$$ = $1;}    

voltage: DOUBLE V                                               {$$ = $1;}
        | /* empty */                                           {$$ = 0.0;}

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