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
    extern Inode::scope_t* current_scope;
    namespace yy {parser::token_type yylex(parser::semantic_type* yylval, parser::location_type* l, driver_t* driver);}
}

%token <std::string> NAME 
%token <int> INTEGER
%token WHILE             "while"   
       INPUT              "?"
       IF                "if"
       SCOLON            ";"
       LCB               "{"
       RCB               "}"
       LRB               "("
       RRB               ")"
       OUTPUT            "print"
       ASSIGN            "="
       OR                "||" 
       AND               "&&"
       NOT               "!"
       EQUAL             "=="
       NOT_EQUAL         "!="
       GREATER           ">"
       LESS              "<"
       LESS_OR_EQUAL     "<="
       GREATER_OR_EQUAL  ">="
       PLUS              "+"  
       MINUS             "-"
       MUL               "*"
       DIV               "/"
       MOD               "%"

%nterm<Inode::scope_t*> scope
%nterm<Inode::scope_t*> close_sc
%nterm<Inode::node_t*> stm
%nterm<Inode::node_t*>  assign
%nterm<Inode::node_t*>  lval
%nterm<Inode::node_t*>  if 
%nterm<Inode::node_t*>  while
%nterm<Inode::node_t*>  expr1
%nterm<Inode::node_t*>  expr2
%nterm<Inode::node_t*>  expr3
%nterm<Inode::node_t*>  condition
%nterm<Inode::node_t*>  output 
%nterm<Inode::node_t*> stms
%nterm<Inode::node_t*> open_sc

%%

program:    stms                                {current_scope->calc();};

scope:      open_sc stms close_sc               {$$ = $3;};

open_sc:    LCB                                 { current_scope = new Inode::scope_t(current_scope); };

close_sc:   RCB                                 {
                                                    $$ = current_scope;
                                                    current_scope = current_scope->get_prev();
                                                };

stms:       stm                                 {current_scope->add_branch($1);};
          | stms stm                            {current_scope->add_branch($2);};
          | stms scope                          {current_scope->add_branch($2);};

stm:        assign                              {$$ = $1;};
          | if                                  {$$ = $1;};
          | while                               {$$ = $1;};
          | output                              {$$ = $1;};

assign:     lval ASSIGN expr1 SCOLON            {$$ = new Inode::bin_op_t($1, Inode::bin_op::assign_, $3);};

lval:       NAME                                {
                                                    Inode::node_t* node = current_scope->visible($1);
                                                    if(!node) {
                                                        node = new Inode::decl_t;
                                                        current_scope->add(node, $1);
                                                    }
                                                    $$ = node;
                                                }

expr1:       expr2 PLUS expr2                   {$$ = new Inode::bin_op_t($1, Inode::bin_op::plus_, $3);};
           | expr2 MINUS expr2                  {$$ = new Inode::bin_op_t($1, Inode::bin_op::minus_, $3);};
           | expr2                              {$$ = $1;}

expr2:      expr3 MUL expr3                     {$$ = new Inode::bin_op_t($1, Inode::bin_op::mult_, $3);};
          | expr3 DIV expr3                     {$$ = new Inode::bin_op_t($1, Inode::bin_op::div_, $3);};
          | expr3 MOD expr3                     {$$ = new Inode::bin_op_t($1, Inode::bin_op::mod_, $3);};
          | expr3                               {$$ = $1;}

expr3:      LRB expr1 RRB                       {$$ = $2;} 
          | NAME                                {
                                                    Inode::node_t* visible = current_scope->visible($1);
                                                    if(!visible) {
                                                        driver->report_runtime_error(@1, driver_t::error_type::RUNTIME_UNDEFINED_NAME);
                                                    } 
                                                    $$ = visible;
                                                };
          | INTEGER                             {$$ = new Inode::integer_t($1);}
          | INPUT                               {$$ = new Inode::unary_op_t(Inode::unary_op::input_, nullptr);};                  

condition:  expr1 AND expr1                     {$$ = new Inode::bin_op_t($1, Inode::bin_op::and_, $3);};
          | expr1 OR expr1                      {$$ = new Inode::bin_op_t($1, Inode::bin_op::or_, $3);};      
          | NOT expr1                           {$$ = new Inode::unary_op_t($2, Inode::unary_op::not_);};    
          | expr1 EQUAL expr1                   {$$ = new Inode::bin_op_t($1, Inode::bin_op::equal_, $3);};  
          | expr1 NOT_EQUAL expr1               {$$ = new Inode::bin_op_t($1, Inode::bin_op::not_equal_, $3);};  
          | expr1 GREATER expr1                 {$$ = new Inode::bin_op_t($1, Inode::bin_op::greater_, $3);};  
          | expr1 LESS expr1                    {$$ = new Inode::bin_op_t($1, Inode::bin_op::less_, $3);};  
          | expr1 GREATER_OR_EQUAL expr1        {$$ = new Inode::bin_op_t($1, Inode::bin_op::greater_or_equal_, $3);};  
          | expr1 LESS_OR_EQUAL expr1           {$$ = new Inode::bin_op_t($1, Inode::bin_op::less_or_equal_, $3);};
          | expr1                               {$$ = $1;}

if:        IF LRB condition RRB scope           {$$ = new Inode::if_t($3, $5);};

while:     WHILE LRB condition RRB scope        {$$ = new Inode::while_t($3, $5);};

output:    OUTPUT expr1 SCOLON                  {$$ = new Inode::unary_op_t(Inode::unary_op::output_, $2);};
                         
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