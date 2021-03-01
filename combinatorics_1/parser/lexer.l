
%option c++
%option noyywrap nounput noinput batch debug
%option yylineno
%option yyclass="mylexer_t"
%{
#include "parser.tab.hh"
#include "mylexer.hpp"

#define YY_USER_ACTION update_location();
%}


WS      [ \t\v\n]+
UNOP    [\+\-]
DIGIT   [0-9]
LBREAK  [\n]+

%%
{UNOP}{0,1}{DIGIT}*              return yy::parser::token_type::INTEGER;
"--"                             return yy::parser::token_type::EDGE;
","                              return yy::parser::token_type::COMMA;
{LBREAK}                         return yy::parser::token_type::LBREAK;
{WS}      
%%