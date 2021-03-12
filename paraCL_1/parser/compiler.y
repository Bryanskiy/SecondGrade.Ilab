%language "c++"
%skeleton "lalr1.cc"

%define api.value.type variant
%define parse.error custom
%param {driver_t* driver}

%locations

%code requires {
#include <string>
#include "../node_interface/node.hpp"
namespace yy { class driver_t; }
}

%code {
#include "../driver/driver.hpp"
extern node::scope_t* current_scope;
namespace yy {parser::token_type yylex(parser::semantic_type* yylval, parser::location_type* l, driver_t* driver);}
}

%token <std::string> VARIABLE 
%token <int> INTEGER
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

%nterm<node::scope_t*> scope
%nterm<node::scope_t*> close_sc
%nterm<node::node_t*> stm
%nterm<node::node_t*>  assign
%nterm<node::node_t*>  lval
%nterm<node::node_t*>  if 
%nterm<node::node_t*>  while
%nterm<node::node_t*>  expr1
%nterm<node::node_t*>  expr2
%nterm<node::node_t*>  expr3
%nterm<node::node_t*>  condition
%nterm<node::node_t*>  output 

%%

program:    stms                                {current_scope->execute();};

scope:      open_sc stms close_sc               {$$ = $3;};

open_sc:    LCB                                 {
                                                    current_scope = new node::scope_t(current_scope);
                                                };

close_sc:   RCB                                 {
                                                    $$ = current_scope;
                                                    current_scope = current_scope->get_parent();
                                                };

stms:       stm                                 {current_scope->add_branch($1);};
          | stms stm                            {current_scope->add_branch($2);};
          | stms scope                          {current_scope->add_branch($2);};

stm:        assign                              {$$ = $1;};
          | if                                  {$$ = $1;};
          | while                               {$$ = $1;};
          | output                              {$$ = $1;};

assign:     lval ASSIGN expr1 SCOLON            {$$ = new node::operator_t($1, node::operator_type::assign_, $3);};

lval:       VARIABLE                            {
                                                    node::variable_t* tmp = static_cast<node::variable_t*>(current_scope->search_global($1));
                                                    if(!tmp) {
                                                        $$ = new node::variable_t{};
                                                    } else {
                                                        $$ = tmp;
                                                    }

                                                    if(!current_scope->search_local($1)) {
                                                        current_scope->make_visible($1, $$);
                                                    }
                                                };

expr1:       expr2 PLUS expr2                   {$$ = new node::operator_t($1, node::operator_type::plus_, $3);};
           | expr2 MINUS expr2                  {$$ = new node::operator_t($1, node::operator_type::minus_, $3);};
           | expr2                              {$$ = $1;}

expr2:      expr3 MUL expr3                     {$$ = new node::operator_t($1, node::operator_type::mult_, $3);};
          | expr3 DIV expr3                     {$$ = new node::operator_t($1, node::operator_type::div_, $3);};
          | expr3 MOD expr3                     {$$ = new node::operator_t($1, node::operator_type::mod_, $3);};
          | expr3                               {$$ = $1;}

expr3:      LRB expr1 RRB                       {$$ = $2;} 
          | VARIABLE                            {
                                                    node::variable_t* tmp = static_cast<node::variable_t*>(current_scope->search_global($1));
                                                    if(!tmp) {
                                                        /* undefined variablr */
                                                    } else {
                                                        $$ = tmp;
                                                    }

                                                    if(!current_scope->search_local($1)) {
                                                        current_scope->make_visible($1, $$);
                                                    }
                                                };

          | INTEGER                             {$$ = new node::integer_t($1);}
          | INPUT                               {$$ = new node::operator_t(nullptr, node::operator_type::input_, nullptr);};                  

condition:  expr1 AND expr1                     {$$ = new node::operator_t($1, node::operator_type::and_, $3);};
          | expr1 OR expr1                      {$$ = new node::operator_t($1, node::operator_type::or_, $3);};      
          | NOT expr1                           {$$ = new node::operator_t($2, node::operator_type::not_, nullptr);};    
          | expr1 EQUAL expr1                   {$$ = new node::operator_t($1, node::operator_type::equal_, $3);};  
          | expr1 NOT_EQUAL expr1               {$$ = new node::operator_t($1, node::operator_type::not_equal_, $3);};  
          | expr1 GREATER expr1                 {$$ = new node::operator_t($1, node::operator_type::greater_, $3);};  
          | expr1 LESS expr1                    {$$ = new node::operator_t($1, node::operator_type::less_, $3);};  
          | expr1 GREATER_OR_EQUAL expr1        {$$ = new node::operator_t($1, node::operator_type::greater_or_equal_, $3);};  
          | expr1 LESS_OR_EQUAL expr1           {$$ = new node::operator_t($1, node::operator_type::less_or_equal_, $3);};
          | expr1                               {$$ = $1;}

if:        IF LRB condition RRB scope           {$$ = new  node::if_t($3, $5);};

while:     WHILE LRB condition RRB scope        {$$ = new node::while_t($3, $5);};

output:    OUTPUT expr1 SCOLON                  {$$ = new node::operator_t(nullptr, node::operator_type::output_, $2);};
                         
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
}