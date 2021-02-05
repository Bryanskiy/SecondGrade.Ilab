%option c++
%option yylineno
%option yyclass="mylexer_t"
%{
#include "compiler.tab.hh"
#include "mylexer.hpp"

#define YY_USER_ACTION update_location();
%}

WS [ \t\n\v]+
INTEGER [0]|[1-9][0-9]*
NAME [a-zA-Z][a-zA-Z0-9]*

%%
"while"     return yy::parser::token_type::WHILE;
"if"        return yy::parser::token_type::IF;

";"         return yy::parser::token_type::SCOLON;
"{"         return yy::parser::token_type::LCB;
"}"         return yy::parser::token_type::RCB;
"("         return yy::parser::token_type::LRB;
")"         return yy::parser::token_type::RRB;

"print"     return yy::parser::token_type::OUTPUT;

">="        return yy::parser::token_type::GREATER_OR_EQUAL;
"<="        return yy::parser::token_type::LESS_OR_EQUAL;
">"         return yy::parser::token_type::GREATER;
"<"         return yy::parser::token_type::LESS;
"=="        return yy::parser::token_type::EQUAL;
"!="        return yy::parser::token_type::NOT_EQUAL;

"="         return yy::parser::token_type::ASSIGN;

"?"         return yy::parser::token_type::INPUT;

"||"        return yy::parser::token_type::OR;
"&&"        return yy::parser::token_type::AND;
"!"         return yy::parser::token_type::NOT;

"+"         return yy::parser::token_type::PLUS;
"-"         return yy::parser::token_type::MINUS;

"*"         return yy::parser::token_type::MUL;
"/"         return yy::parser::token_type::DIV;
"%"         return yy::parser::token_type::MOD;

{WS}        
{INTEGER}   return yy::parser::token_type::INTEGER;
{NAME}      return yy::parser::token_type::NAME;
                
%%