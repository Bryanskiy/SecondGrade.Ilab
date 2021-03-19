// A Bison parser, made by GNU Bison 3.7.5.312-f0811.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.





#include "compiler.tab.hh"


// Unqualified %code blocks.
#line 16 "compiler.y"

    #include "../driver/driver.hpp"
    extern Inode::scope_t* current_scope;
    namespace yy {parser::token_type yylex(parser::semantic_type* yylval, parser::location_type* l, driver_t* driver);}

#line 52 "compiler.tab.cc"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace yy {
#line 144 "compiler.tab.cc"

  /// Build a parser object.
  parser::parser (driver_t* driver_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      driver (driver_yyarg)
  {}

  parser::~parser ()
  {}

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | symbol kinds.  |
  `---------------*/

  // basic_symbol.
  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
    , location (that.location)
  {
    switch (this->kind ())
    {
      case symbol_kind::S_open_sc: // open_sc
      case symbol_kind::S_stms: // stms
      case symbol_kind::S_stm: // stm
      case symbol_kind::S_assign: // assign
      case symbol_kind::S_lval: // lval
      case symbol_kind::S_expr1: // expr1
      case symbol_kind::S_expr2: // expr2
      case symbol_kind::S_expr3: // expr3
      case symbol_kind::S_condition: // condition
      case symbol_kind::S_if: // if
      case symbol_kind::S_while: // while
      case symbol_kind::S_output: // output
        value.copy< Inode::node_t* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_scope: // scope
      case symbol_kind::S_close_sc: // close_sc
        value.copy< Inode::scope_t* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.copy< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NAME: // NAME
        value.copy< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }



  template <typename Base>
  parser::symbol_kind_type
  parser::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }

  template <typename Base>
  bool
  parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->kind ())
    {
      case symbol_kind::S_open_sc: // open_sc
      case symbol_kind::S_stms: // stms
      case symbol_kind::S_stm: // stm
      case symbol_kind::S_assign: // assign
      case symbol_kind::S_lval: // lval
      case symbol_kind::S_expr1: // expr1
      case symbol_kind::S_expr2: // expr2
      case symbol_kind::S_expr3: // expr3
      case symbol_kind::S_condition: // condition
      case symbol_kind::S_if: // if
      case symbol_kind::S_while: // while
      case symbol_kind::S_output: // output
        value.move< Inode::node_t* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_scope: // scope
      case symbol_kind::S_close_sc: // close_sc
        value.move< Inode::scope_t* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< int > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_NAME: // NAME
        value.move< std::string > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

    location = YY_MOVE (s.location);
  }

  // by_kind.
  parser::by_kind::by_kind ()
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  parser::by_kind::by_kind (by_kind&& that)
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  parser::by_kind::by_kind (const by_kind& that)
    : kind_ (that.kind_)
  {}

  parser::by_kind::by_kind (token_kind_type t)
    : kind_ (yytranslate_ (t))
  {}

  void
  parser::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  void
  parser::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  parser::symbol_kind_type
  parser::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }

  parser::symbol_kind_type
  parser::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  // by_state.
  parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  parser::symbol_kind_type
  parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_open_sc: // open_sc
      case symbol_kind::S_stms: // stms
      case symbol_kind::S_stm: // stm
      case symbol_kind::S_assign: // assign
      case symbol_kind::S_lval: // lval
      case symbol_kind::S_expr1: // expr1
      case symbol_kind::S_expr2: // expr2
      case symbol_kind::S_expr3: // expr3
      case symbol_kind::S_condition: // condition
      case symbol_kind::S_if: // if
      case symbol_kind::S_while: // while
      case symbol_kind::S_output: // output
        value.YY_MOVE_OR_COPY< Inode::node_t* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_scope: // scope
      case symbol_kind::S_close_sc: // close_sc
        value.YY_MOVE_OR_COPY< Inode::scope_t* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NAME: // NAME
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_open_sc: // open_sc
      case symbol_kind::S_stms: // stms
      case symbol_kind::S_stm: // stm
      case symbol_kind::S_assign: // assign
      case symbol_kind::S_lval: // lval
      case symbol_kind::S_expr1: // expr1
      case symbol_kind::S_expr2: // expr2
      case symbol_kind::S_expr3: // expr3
      case symbol_kind::S_condition: // condition
      case symbol_kind::S_if: // if
      case symbol_kind::S_while: // while
      case symbol_kind::S_output: // output
        value.move< Inode::node_t* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_scope: // scope
      case symbol_kind::S_close_sc: // close_sc
        value.move< Inode::scope_t* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NAME: // NAME
        value.move< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_open_sc: // open_sc
      case symbol_kind::S_stms: // stms
      case symbol_kind::S_stm: // stm
      case symbol_kind::S_assign: // assign
      case symbol_kind::S_lval: // lval
      case symbol_kind::S_expr1: // expr1
      case symbol_kind::S_expr2: // expr2
      case symbol_kind::S_expr3: // expr3
      case symbol_kind::S_condition: // condition
      case symbol_kind::S_if: // if
      case symbol_kind::S_while: // while
      case symbol_kind::S_output: // output
        value.copy< Inode::node_t* > (that.value);
        break;

      case symbol_kind::S_scope: // scope
      case symbol_kind::S_close_sc: // close_sc
        value.copy< Inode::scope_t* > (that.value);
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.copy< int > (that.value);
        break;

      case symbol_kind::S_NAME: // NAME
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_open_sc: // open_sc
      case symbol_kind::S_stms: // stms
      case symbol_kind::S_stm: // stm
      case symbol_kind::S_assign: // assign
      case symbol_kind::S_lval: // lval
      case symbol_kind::S_expr1: // expr1
      case symbol_kind::S_expr2: // expr2
      case symbol_kind::S_expr3: // expr3
      case symbol_kind::S_condition: // condition
      case symbol_kind::S_if: // if
      case symbol_kind::S_while: // while
      case symbol_kind::S_output: // output
        value.move< Inode::node_t* > (that.value);
        break;

      case symbol_kind::S_scope: // scope
      case symbol_kind::S_close_sc: // close_sc
        value.move< Inode::scope_t* > (that.value);
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< int > (that.value);
        break;

      case symbol_kind::S_NAME: // NAME
        value.move< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
  }

  int
  parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.kind_ = yytranslate_ (yylex (&yyla.value, &yyla.location, driver));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_open_sc: // open_sc
      case symbol_kind::S_stms: // stms
      case symbol_kind::S_stm: // stm
      case symbol_kind::S_assign: // assign
      case symbol_kind::S_lval: // lval
      case symbol_kind::S_expr1: // expr1
      case symbol_kind::S_expr2: // expr2
      case symbol_kind::S_expr3: // expr3
      case symbol_kind::S_condition: // condition
      case symbol_kind::S_if: // if
      case symbol_kind::S_while: // while
      case symbol_kind::S_output: // output
        yylhs.value.emplace< Inode::node_t* > ();
        break;

      case symbol_kind::S_scope: // scope
      case symbol_kind::S_close_sc: // close_sc
        yylhs.value.emplace< Inode::scope_t* > ();
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        yylhs.value.emplace< int > ();
        break;

      case symbol_kind::S_NAME: // NAME
        yylhs.value.emplace< std::string > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // program: stms
#line 54 "compiler.y"
                                                {current_scope->calc();}
#line 823 "compiler.tab.cc"
    break;

  case 3: // scope: open_sc stms close_sc
#line 56 "compiler.y"
                                                {yylhs.value.as < Inode::scope_t* > () = yystack_[0].value.as < Inode::scope_t* > ();}
#line 829 "compiler.tab.cc"
    break;

  case 4: // open_sc: LCB
#line 58 "compiler.y"
                                                { current_scope = new Inode::scope_t(current_scope); }
#line 835 "compiler.tab.cc"
    break;

  case 5: // close_sc: RCB
#line 60 "compiler.y"
                                                {
                                                    yylhs.value.as < Inode::scope_t* > () = current_scope;
                                                    current_scope = current_scope->get_prev();
                                                }
#line 844 "compiler.tab.cc"
    break;

  case 6: // stms: stm
#line 65 "compiler.y"
                                                {current_scope->add_branch(yystack_[0].value.as < Inode::node_t* > ());}
#line 850 "compiler.tab.cc"
    break;

  case 7: // stms: stms stm
#line 66 "compiler.y"
                                                {current_scope->add_branch(yystack_[0].value.as < Inode::node_t* > ());}
#line 856 "compiler.tab.cc"
    break;

  case 8: // stms: stms scope
#line 67 "compiler.y"
                                                {current_scope->add_branch(yystack_[0].value.as < Inode::scope_t* > ());}
#line 862 "compiler.tab.cc"
    break;

  case 9: // stm: assign
#line 69 "compiler.y"
                                                {yylhs.value.as < Inode::node_t* > () = yystack_[0].value.as < Inode::node_t* > ();}
#line 868 "compiler.tab.cc"
    break;

  case 10: // stm: if
#line 70 "compiler.y"
                                                {yylhs.value.as < Inode::node_t* > () = yystack_[0].value.as < Inode::node_t* > ();}
#line 874 "compiler.tab.cc"
    break;

  case 11: // stm: while
#line 71 "compiler.y"
                                                {yylhs.value.as < Inode::node_t* > () = yystack_[0].value.as < Inode::node_t* > ();}
#line 880 "compiler.tab.cc"
    break;

  case 12: // stm: output
#line 72 "compiler.y"
                                                {yylhs.value.as < Inode::node_t* > () = yystack_[0].value.as < Inode::node_t* > ();}
#line 886 "compiler.tab.cc"
    break;

  case 13: // assign: lval ASSIGN expr1 SCOLON
#line 74 "compiler.y"
                                                {yylhs.value.as < Inode::node_t* > () = new Inode::bin_op_t(yystack_[3].value.as < Inode::node_t* > (), Inode::bin_op::assign_, yystack_[1].value.as < Inode::node_t* > ());}
#line 892 "compiler.tab.cc"
    break;

  case 14: // lval: NAME
#line 76 "compiler.y"
                                                {yylhs.value.as < Inode::node_t* > () = current_scope->add(yystack_[0].value.as < std::string > ());}
#line 898 "compiler.tab.cc"
    break;

  case 15: // expr1: expr2 PLUS expr2
#line 78 "compiler.y"
                                                {yylhs.value.as < Inode::node_t* > () = new Inode::bin_op_t(yystack_[2].value.as < Inode::node_t* > (), Inode::bin_op::plus_, yystack_[0].value.as < Inode::node_t* > ());}
#line 904 "compiler.tab.cc"
    break;

  case 16: // expr1: expr2 MINUS expr2
#line 79 "compiler.y"
                                                {yylhs.value.as < Inode::node_t* > () = new Inode::bin_op_t(yystack_[2].value.as < Inode::node_t* > (), Inode::bin_op::minus_, yystack_[0].value.as < Inode::node_t* > ());}
#line 910 "compiler.tab.cc"
    break;

  case 17: // expr1: expr2
#line 80 "compiler.y"
                                                {yylhs.value.as < Inode::node_t* > () = yystack_[0].value.as < Inode::node_t* > ();}
#line 916 "compiler.tab.cc"
    break;

  case 18: // expr2: expr3 MUL expr3
#line 82 "compiler.y"
                                                {yylhs.value.as < Inode::node_t* > () = new Inode::bin_op_t(yystack_[2].value.as < Inode::node_t* > (), Inode::bin_op::mult_, yystack_[0].value.as < Inode::node_t* > ());}
#line 922 "compiler.tab.cc"
    break;

  case 19: // expr2: expr3 DIV expr3
#line 83 "compiler.y"
                                                {yylhs.value.as < Inode::node_t* > () = new Inode::bin_op_t(yystack_[2].value.as < Inode::node_t* > (), Inode::bin_op::div_, yystack_[0].value.as < Inode::node_t* > ());}
#line 928 "compiler.tab.cc"
    break;

  case 20: // expr2: expr3 MOD expr3
#line 84 "compiler.y"
                                                {yylhs.value.as < Inode::node_t* > () = new Inode::bin_op_t(yystack_[2].value.as < Inode::node_t* > (), Inode::bin_op::mod_, yystack_[0].value.as < Inode::node_t* > ());}
#line 934 "compiler.tab.cc"
    break;

  case 21: // expr2: expr3
#line 85 "compiler.y"
                                                {yylhs.value.as < Inode::node_t* > () = yystack_[0].value.as < Inode::node_t* > ();}
#line 940 "compiler.tab.cc"
    break;

  case 22: // expr3: LRB expr1 RRB
#line 87 "compiler.y"
                                                {yylhs.value.as < Inode::node_t* > () = yystack_[1].value.as < Inode::node_t* > ();}
#line 946 "compiler.tab.cc"
    break;

  case 23: // expr3: NAME
#line 88 "compiler.y"
                                                {yylhs.value.as < Inode::node_t* > () = current_scope->visible(yystack_[0].value.as < std::string > ());}
#line 952 "compiler.tab.cc"
    break;

  case 24: // expr3: INTEGER
#line 89 "compiler.y"
                                                {yylhs.value.as < Inode::node_t* > () = new Inode::integer_t(yystack_[0].value.as < int > ());}
#line 958 "compiler.tab.cc"
    break;

  case 25: // expr3: INPUT
#line 90 "compiler.y"
                                                {yylhs.value.as < Inode::node_t* > () = new Inode::unary_op_t(Inode::unary_op::input_, nullptr);}
#line 964 "compiler.tab.cc"
    break;

  case 26: // condition: expr1 AND expr1
#line 92 "compiler.y"
                                                {yylhs.value.as < Inode::node_t* > () = new Inode::bin_op_t(yystack_[2].value.as < Inode::node_t* > (), Inode::bin_op::and_, yystack_[0].value.as < Inode::node_t* > ());}
#line 970 "compiler.tab.cc"
    break;

  case 27: // condition: expr1 OR expr1
#line 93 "compiler.y"
                                                {yylhs.value.as < Inode::node_t* > () = new Inode::bin_op_t(yystack_[2].value.as < Inode::node_t* > (), Inode::bin_op::or_, yystack_[0].value.as < Inode::node_t* > ());}
#line 976 "compiler.tab.cc"
    break;

  case 28: // condition: NOT expr1
#line 94 "compiler.y"
                                                {yylhs.value.as < Inode::node_t* > () = new Inode::unary_op_t(yystack_[0].value.as < Inode::node_t* > (), Inode::unary_op::not_);}
#line 982 "compiler.tab.cc"
    break;

  case 29: // condition: expr1 EQUAL expr1
#line 95 "compiler.y"
                                                {yylhs.value.as < Inode::node_t* > () = new Inode::bin_op_t(yystack_[2].value.as < Inode::node_t* > (), Inode::bin_op::equal_, yystack_[0].value.as < Inode::node_t* > ());}
#line 988 "compiler.tab.cc"
    break;

  case 30: // condition: expr1 NOT_EQUAL expr1
#line 96 "compiler.y"
                                                {yylhs.value.as < Inode::node_t* > () = new Inode::bin_op_t(yystack_[2].value.as < Inode::node_t* > (), Inode::bin_op::not_equal_, yystack_[0].value.as < Inode::node_t* > ());}
#line 994 "compiler.tab.cc"
    break;

  case 31: // condition: expr1 GREATER expr1
#line 97 "compiler.y"
                                                {yylhs.value.as < Inode::node_t* > () = new Inode::bin_op_t(yystack_[2].value.as < Inode::node_t* > (), Inode::bin_op::greater_, yystack_[0].value.as < Inode::node_t* > ());}
#line 1000 "compiler.tab.cc"
    break;

  case 32: // condition: expr1 LESS expr1
#line 98 "compiler.y"
                                                {yylhs.value.as < Inode::node_t* > () = new Inode::bin_op_t(yystack_[2].value.as < Inode::node_t* > (), Inode::bin_op::less_, yystack_[0].value.as < Inode::node_t* > ());}
#line 1006 "compiler.tab.cc"
    break;

  case 33: // condition: expr1 GREATER_OR_EQUAL expr1
#line 99 "compiler.y"
                                                {yylhs.value.as < Inode::node_t* > () = new Inode::bin_op_t(yystack_[2].value.as < Inode::node_t* > (), Inode::bin_op::greater_or_equal_, yystack_[0].value.as < Inode::node_t* > ());}
#line 1012 "compiler.tab.cc"
    break;

  case 34: // condition: expr1 LESS_OR_EQUAL expr1
#line 100 "compiler.y"
                                                {yylhs.value.as < Inode::node_t* > () = new Inode::bin_op_t(yystack_[2].value.as < Inode::node_t* > (), Inode::bin_op::less_or_equal_, yystack_[0].value.as < Inode::node_t* > ());}
#line 1018 "compiler.tab.cc"
    break;

  case 35: // condition: expr1
#line 101 "compiler.y"
                                                {yylhs.value.as < Inode::node_t* > () = yystack_[0].value.as < Inode::node_t* > ();}
#line 1024 "compiler.tab.cc"
    break;

  case 36: // if: IF LRB condition RRB scope
#line 103 "compiler.y"
                                                {yylhs.value.as < Inode::node_t* > () = new Inode::if_t(yystack_[2].value.as < Inode::node_t* > (), yystack_[0].value.as < Inode::scope_t* > ());}
#line 1030 "compiler.tab.cc"
    break;

  case 37: // while: WHILE LRB condition RRB scope
#line 105 "compiler.y"
                                                {yylhs.value.as < Inode::node_t* > () = new Inode::while_t(yystack_[2].value.as < Inode::node_t* > (), yystack_[0].value.as < Inode::scope_t* > ());}
#line 1036 "compiler.tab.cc"
    break;

  case 38: // output: OUTPUT expr1 SCOLON
#line 107 "compiler.y"
                                                 {yylhs.value.as < Inode::node_t* > () = new Inode::unary_op_t(Inode::unary_op::output_, yystack_[1].value.as < Inode::node_t* > ());}
#line 1042 "compiler.tab.cc"
    break;


#line 1046 "compiler.tab.cc"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        context yyctx (*this, yyla);
        report_syntax_error (yyctx);
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yyerror_range[1].location = yystack_[0].location;
        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  const char *
  parser::symbol_name (symbol_kind_type yysymbol)
  {
    static const char *const yy_sname[] =
    {
    "end of file", "error", "invalid token", "NAME", "INTEGER", "WHILE",
  "IF", "SCOLON", "LCB", "RCB", "LRB", "RRB", "OUTPUT", "ASSIGN", "INPUT",
  "OR", "AND", "NOT", "EQUAL", "NOT_EQUAL", "GREATER", "LESS",
  "LESS_OR_EQUAL", "GREATER_OR_EQUAL", "PLUS", "MINUS", "MUL", "DIV",
  "MOD", "$accept", "program", "scope", "open_sc", "close_sc", "stms",
  "stm", "assign", "lval", "expr1", "expr2", "expr3", "condition", "if",
  "while", "output", YY_NULLPTR
    };
    return yy_sname[yysymbol];
  }



  // parser::context.
  parser::context::context (const parser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  parser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

    int yyn = yypact_[+yyparser_.yystack_[0].state];
    if (!yy_pact_value_is_default_ (yyn))
      {
        /* Start YYX at -YYN if negative to avoid negative indexes in
           YYCHECK.  In other words, skip the first -YYN actions for
           this state because they are default actions.  */
        int yyxbegin = yyn < 0 ? -yyn : 0;
        // Stay within bounds of both yycheck and yytname.
        int yychecklim = yylast_ - yyn + 1;
        int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
          if (yycheck_[yyx + yyn] == yyx && yyx != symbol_kind::S_YYerror
              && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
            {
              if (!yyarg)
                ++yycount;
              else if (yycount == yyargn)
                return 0;
              else
                yyarg[yycount++] = YY_CAST (symbol_kind_type, yyx);
            }
      }

    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }




  const signed char parser::yypact_ninf_ = -20;

  const signed char parser::yytable_ninf_ = -1;

  const signed char
  parser::yypact_[] =
  {
      15,   -20,    -8,     1,     2,    19,    43,   -20,   -20,     9,
     -20,   -20,   -20,     0,     0,   -20,   -20,     2,   -20,     8,
      12,   -19,   -20,   -20,   -20,    15,   -20,     2,     2,    38,
      14,    22,    36,   -20,     2,     2,     2,     2,     2,    23,
      45,   -20,     2,     2,     2,     2,     2,     2,     2,     2,
      42,    42,   -20,   -20,   -20,   -20,   -20,   -20,   -20,   -20,
     -20,   -20,   -20,   -20,   -20,   -20,   -20,   -20,   -20,   -20,
     -20
  };

  const signed char
  parser::yydefact_[] =
  {
       0,    14,     0,     0,     0,     0,     2,     6,     9,     0,
      10,    11,    12,     0,     0,    23,    24,     0,    25,     0,
      17,    21,     1,     4,     8,     0,     7,     0,     0,    35,
       0,     0,     0,    38,     0,     0,     0,     0,     0,     0,
       0,    28,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    22,    15,    16,    18,    19,    20,     5,     3,
      13,    27,    26,    29,    30,    31,    32,    34,    33,    37,
      36
  };

  const signed char
  parser::yypgoto_[] =
  {
     -20,   -20,    16,   -20,   -20,     5,    -5,   -20,   -20,    -4,
      34,    26,    51,   -20,   -20,   -20
  };

  const signed char
  parser::yydefgoto_[] =
  {
       0,     5,    24,    25,    59,     6,     7,     8,     9,    29,
      20,    21,    30,    10,    11,    12
  };

  const signed char
  parser::yytable_[] =
  {
      19,    26,    13,    15,    16,    15,    16,    36,    37,    38,
      17,    14,    17,    32,    18,    33,    18,    28,     1,    22,
       2,     3,    27,    40,    41,    50,     1,     4,     2,     3,
      39,    23,    58,    51,    26,     4,    34,    35,    61,    62,
      63,    64,    65,    66,    67,    68,     1,    52,     2,     3,
      23,    23,    60,    42,    43,     4,    44,    45,    46,    47,
      48,    49,    55,    56,    57,    31,    69,    70,    53,    54
  };

  const signed char
  parser::yycheck_[] =
  {
       4,     6,    10,     3,     4,     3,     4,    26,    27,    28,
      10,    10,    10,    17,    14,     7,    14,    17,     3,     0,
       5,     6,    13,    27,    28,    11,     3,    12,     5,     6,
      25,     8,     9,    11,    39,    12,    24,    25,    42,    43,
      44,    45,    46,    47,    48,    49,     3,    11,     5,     6,
       8,     8,     7,    15,    16,    12,    18,    19,    20,    21,
      22,    23,    36,    37,    38,    14,    50,    51,    34,    35
  };

  const signed char
  parser::yystos_[] =
  {
       0,     3,     5,     6,    12,    30,    34,    35,    36,    37,
      42,    43,    44,    10,    10,     3,     4,    10,    14,    38,
      39,    40,     0,     8,    31,    32,    35,    13,    17,    38,
      41,    41,    38,     7,    24,    25,    26,    27,    28,    34,
      38,    38,    15,    16,    18,    19,    20,    21,    22,    23,
      11,    11,    11,    39,    39,    40,    40,    40,     9,    33,
       7,    38,    38,    38,    38,    38,    38,    38,    38,    31,
      31
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    29,    30,    31,    32,    33,    34,    34,    34,    35,
      35,    35,    35,    36,    37,    38,    38,    38,    39,    39,
      39,    39,    40,    40,    40,    40,    41,    41,    41,    41,
      41,    41,    41,    41,    41,    41,    42,    43,    44
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     1,     3,     1,     1,     1,     2,     2,     1,
       1,     1,     1,     4,     1,     3,     3,     1,     3,     3,
       3,     1,     3,     1,     1,     1,     3,     3,     2,     3,
       3,     3,     3,     3,     3,     1,     5,     5,     3
  };




#if YYDEBUG
  const signed char
  parser::yyrline_[] =
  {
       0,    54,    54,    56,    58,    60,    65,    66,    67,    69,
      70,    71,    72,    74,    76,    78,    79,    80,    82,    83,
      84,    85,    87,    88,    89,    90,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   103,   105,   107
  };

  void
  parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  parser::symbol_kind_type
  parser::yytranslate_ (int t)
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const signed char
    translate_table[] =
    {
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28
    };
    // Last valid token kind.
    const int code_max = 283;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

} // yy
#line 1474 "compiler.tab.cc"

#line 109 "compiler.y"


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
