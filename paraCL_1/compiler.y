%language "c++"

%skeleton "lalr1.cc"
%param {yy::driver_t* driver}
%code requires
{
#include <string>

#include "Inode.hpp"

struct yyret {
    std::string name_;
    int integer_;
    Inode::Inode_t* node_;
};

#define YYSTYPE yyret

// forward decl of argument to parser
namespace yy { class driver_t; }
}

%code
{
#include "parser.hpp"
Inode::Iscope_t* current_scope = Inode::make_scope();
namespace yy {parser::token_type yylex(parser::semantic_type* yylval, driver_t* driver);}
}

%token NAME INTEGER
%token WHILE IF
%token SCOLON LCB RCB LRB RRB
%token OUTPUT
%right ASSIGN
%left INPUT
%left OR
%left AND
%left NOT
%left EQUAL NOT_EQUAL
%left GREATER LESS LESS_OR_EQUAL GREATER_OR_EQUAL
%left PLUS MINUS
%left MUL DIV MOD


%%

program:    stms                                {current_scope->calc();};

scope:      open_sc stms close_sc               {$$ = $3;};

open_sc:    LCB                                 {
                                                    current_scope = current_scope->duplicate();
                                                };

close_sc:   RCB                                 {
                                                    $$.node_ = current_scope;
                                                    current_scope = current_scope->reset();
                                                };

stms:       stm                                 {current_scope->add_branch($1.node_);};
          | stms stm                            {current_scope->add_branch($2.node_);};
          | stms scope                          {current_scope->add_branch($2.node_);};

stm:        assign                              {$$ = $1;};
          | if                                  {$$ = $1;};
          | while                               {$$ = $1;};
          | output                              {$$ = $1;};

assign:     lval ASSIGN expr SCOLON             {$$.node_ = Inode::make_bin_op($1.node_, Inode::bin_op::assign_, $3.node_);};

lval:       NAME                                {$$.node_ = current_scope->add($1.name_);};

expr:       expr PLUS expr                      {$$.node_ = Inode::make_bin_op($1.node_, Inode::bin_op::plus_, $3.node_);};
          | expr MINUS expr                     {$$.node_ = Inode::make_bin_op($1.node_, Inode::bin_op::minus_, $3.node_);};
          | expr MUL expr                       {$$.node_ = Inode::make_bin_op($1.node_, Inode::bin_op::mult_, $3.node_);};
          | expr DIV expr                       {$$.node_ = Inode::make_bin_op($1.node_, Inode::bin_op::div_, $3.node_);};
          | expr MOD expr                       {$$.node_ = Inode::make_bin_op($1.node_, Inode::bin_op::mod_, $3.node_);};
          | expr AND expr                       {$$.node_ = Inode::make_bin_op($1.node_, Inode::bin_op::and_, $3.node_);};
          | expr OR expr                        {$$.node_ = Inode::make_bin_op($1.node_, Inode::bin_op::or_, $3.node_);};      
          | NOT expr                            {$$.node_ = Inode::make_unary_op($2.node_, Inode::unary_op::not_);};    
          | expr EQUAL expr                     {$$.node_ = Inode::make_bin_op($1.node_, Inode::bin_op::equal_, $3.node_);};  
          | expr NOT_EQUAL expr                 {$$.node_ = Inode::make_bin_op($1.node_, Inode::bin_op::not_equal_, $3.node_);};  
          | expr GREATER expr                   {$$.node_ = Inode::make_bin_op($1.node_, Inode::bin_op::greater_, $3.node_);};  
          | expr LESS expr                      {$$.node_ = Inode::make_bin_op($1.node_, Inode::bin_op::less_, $3.node_);};  
          | expr GREATER_OR_EQUAL expr          {$$.node_ = Inode::make_bin_op($1.node_, Inode::bin_op::greater_or_equal_, $3.node_);};  
          | expr LESS_OR_EQUAL expr             {$$.node_ = Inode::make_bin_op($1.node_, Inode::bin_op::less_or_equal_, $3.node_);};  
          | NAME                                {$$.node_ = current_scope->visible($1.name_);};
          | INTEGER                             {$$.node_ = Inode::make_value($1.integer_);}
          | INPUT                               {$$.node_ = Inode::make_unary_op(Inode::unary_op::input_, nullptr);};                  

if:        IF LRB expr RRB scope                {$$.node_ = Inode::make_if($3.node_, $5.node_);};

while:     WHILE LRB expr RRB scope             {$$.node_ = Inode::make_while($3.node_, $5.node_);};

output:    OUTPUT expr SCOLON                   {$$.node_ = Inode::make_unary_op(Inode::unary_op::output_, $2.node_);};
                         
%%

namespace yy {
parser::token_type yylex(parser::semantic_type* yylval, driver_t* driver) {return driver->yylex(yylval);
}

void parser::error(const std::string& s) {std::cout << s << std::endl;}
}