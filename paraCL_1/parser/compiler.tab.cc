// A Bison parser, made by GNU Bison 3.7.1.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2020 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
extern Inode::Iscope_t* current_scope;
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
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE (Symbol)
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
      case symbol_kind::S_stm: // stm
      case symbol_kind::S_assign: // assign
      case symbol_kind::S_lval: // lval
      case symbol_kind::S_if: // if
      case symbol_kind::S_while: // while
      case symbol_kind::S_expr: // expr
      case symbol_kind::S_output: // output
      case symbol_kind::S_stms: // stms
      case symbol_kind::S_open_sc: // open_sc
        value.copy< Inode::Inode_t* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_scope: // scope
      case symbol_kind::S_close_sc: // close_sc
        value.copy< Inode::Iscope_t* > (YY_MOVE (that.value));
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
      case symbol_kind::S_stm: // stm
      case symbol_kind::S_assign: // assign
      case symbol_kind::S_lval: // lval
      case symbol_kind::S_if: // if
      case symbol_kind::S_while: // while
      case symbol_kind::S_expr: // expr
      case symbol_kind::S_output: // output
      case symbol_kind::S_stms: // stms
      case symbol_kind::S_open_sc: // open_sc
        value.move< Inode::Inode_t* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_scope: // scope
      case symbol_kind::S_close_sc: // close_sc
        value.move< Inode::Iscope_t* > (YY_MOVE (s.value));
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
  parser::by_kind::clear ()
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
      case symbol_kind::S_stm: // stm
      case symbol_kind::S_assign: // assign
      case symbol_kind::S_lval: // lval
      case symbol_kind::S_if: // if
      case symbol_kind::S_while: // while
      case symbol_kind::S_expr: // expr
      case symbol_kind::S_output: // output
      case symbol_kind::S_stms: // stms
      case symbol_kind::S_open_sc: // open_sc
        value.YY_MOVE_OR_COPY< Inode::Inode_t* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_scope: // scope
      case symbol_kind::S_close_sc: // close_sc
        value.YY_MOVE_OR_COPY< Inode::Iscope_t* > (YY_MOVE (that.value));
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
      case symbol_kind::S_stm: // stm
      case symbol_kind::S_assign: // assign
      case symbol_kind::S_lval: // lval
      case symbol_kind::S_if: // if
      case symbol_kind::S_while: // while
      case symbol_kind::S_expr: // expr
      case symbol_kind::S_output: // output
      case symbol_kind::S_stms: // stms
      case symbol_kind::S_open_sc: // open_sc
        value.move< Inode::Inode_t* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_scope: // scope
      case symbol_kind::S_close_sc: // close_sc
        value.move< Inode::Iscope_t* > (YY_MOVE (that.value));
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
      case symbol_kind::S_stm: // stm
      case symbol_kind::S_assign: // assign
      case symbol_kind::S_lval: // lval
      case symbol_kind::S_if: // if
      case symbol_kind::S_while: // while
      case symbol_kind::S_expr: // expr
      case symbol_kind::S_output: // output
      case symbol_kind::S_stms: // stms
      case symbol_kind::S_open_sc: // open_sc
        value.copy< Inode::Inode_t* > (that.value);
        break;

      case symbol_kind::S_scope: // scope
      case symbol_kind::S_close_sc: // close_sc
        value.copy< Inode::Iscope_t* > (that.value);
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
      case symbol_kind::S_stm: // stm
      case symbol_kind::S_assign: // assign
      case symbol_kind::S_lval: // lval
      case symbol_kind::S_if: // if
      case symbol_kind::S_while: // while
      case symbol_kind::S_expr: // expr
      case symbol_kind::S_output: // output
      case symbol_kind::S_stms: // stms
      case symbol_kind::S_open_sc: // open_sc
        value.move< Inode::Inode_t* > (that.value);
        break;

      case symbol_kind::S_scope: // scope
      case symbol_kind::S_close_sc: // close_sc
        value.move< Inode::Iscope_t* > (that.value);
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
    YYUSE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        YYUSE (yykind);
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
      case symbol_kind::S_stm: // stm
      case symbol_kind::S_assign: // assign
      case symbol_kind::S_lval: // lval
      case symbol_kind::S_if: // if
      case symbol_kind::S_while: // while
      case symbol_kind::S_expr: // expr
      case symbol_kind::S_output: // output
      case symbol_kind::S_stms: // stms
      case symbol_kind::S_open_sc: // open_sc
        yylhs.value.emplace< Inode::Inode_t* > ();
        break;

      case symbol_kind::S_scope: // scope
      case symbol_kind::S_close_sc: // close_sc
        yylhs.value.emplace< Inode::Iscope_t* > ();
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
#line 51 "compiler.y"
                                                {current_scope->calc();}
#line 802 "compiler.tab.cc"
    break;

  case 3: // scope: open_sc stms close_sc
#line 53 "compiler.y"
                                                {yylhs.value.as < Inode::Iscope_t* > () = yystack_[0].value.as < Inode::Iscope_t* > ();}
#line 808 "compiler.tab.cc"
    break;

  case 4: // open_sc: LCB
#line 55 "compiler.y"
                                                {
                                                    current_scope = current_scope->duplicate();
                                                }
#line 816 "compiler.tab.cc"
    break;

  case 5: // close_sc: RCB
#line 59 "compiler.y"
                                                {
                                                    yylhs.value.as < Inode::Iscope_t* > () = current_scope;
                                                    current_scope = current_scope->get_prev();
                                                }
#line 825 "compiler.tab.cc"
    break;

  case 6: // stms: stm
#line 64 "compiler.y"
                                                {current_scope->add_branch(yystack_[0].value.as < Inode::Inode_t* > ());}
#line 831 "compiler.tab.cc"
    break;

  case 7: // stms: stms stm
#line 65 "compiler.y"
                                                {current_scope->add_branch(yystack_[0].value.as < Inode::Inode_t* > ());}
#line 837 "compiler.tab.cc"
    break;

  case 8: // stms: stms scope
#line 66 "compiler.y"
                                                {current_scope->add_branch(yystack_[0].value.as < Inode::Iscope_t* > ());}
#line 843 "compiler.tab.cc"
    break;

  case 9: // stm: assign
#line 68 "compiler.y"
                                                {yylhs.value.as < Inode::Inode_t* > () = yystack_[0].value.as < Inode::Inode_t* > ();}
#line 849 "compiler.tab.cc"
    break;

  case 10: // stm: if
#line 69 "compiler.y"
                                                {yylhs.value.as < Inode::Inode_t* > () = yystack_[0].value.as < Inode::Inode_t* > ();}
#line 855 "compiler.tab.cc"
    break;

  case 11: // stm: while
#line 70 "compiler.y"
                                                {yylhs.value.as < Inode::Inode_t* > () = yystack_[0].value.as < Inode::Inode_t* > ();}
#line 861 "compiler.tab.cc"
    break;

  case 12: // stm: output
#line 71 "compiler.y"
                                                {yylhs.value.as < Inode::Inode_t* > () = yystack_[0].value.as < Inode::Inode_t* > ();}
#line 867 "compiler.tab.cc"
    break;

  case 13: // assign: lval ASSIGN expr SCOLON
#line 73 "compiler.y"
                                                {yylhs.value.as < Inode::Inode_t* > () = Inode::make_bin_op(yystack_[3].value.as < Inode::Inode_t* > (), Inode::bin_op::assign_, yystack_[1].value.as < Inode::Inode_t* > ());}
#line 873 "compiler.tab.cc"
    break;

  case 14: // lval: NAME
#line 75 "compiler.y"
                                                {yylhs.value.as < Inode::Inode_t* > () = current_scope->add(yystack_[0].value.as < std::string > ());}
#line 879 "compiler.tab.cc"
    break;

  case 15: // expr: expr PLUS expr
#line 77 "compiler.y"
                                                {yylhs.value.as < Inode::Inode_t* > () = Inode::make_bin_op(yystack_[2].value.as < Inode::Inode_t* > (), Inode::bin_op::plus_, yystack_[0].value.as < Inode::Inode_t* > ());}
#line 885 "compiler.tab.cc"
    break;

  case 16: // expr: expr MINUS expr
#line 78 "compiler.y"
                                                {yylhs.value.as < Inode::Inode_t* > () = Inode::make_bin_op(yystack_[2].value.as < Inode::Inode_t* > (), Inode::bin_op::minus_, yystack_[0].value.as < Inode::Inode_t* > ());}
#line 891 "compiler.tab.cc"
    break;

  case 17: // expr: expr MUL expr
#line 79 "compiler.y"
                                                {yylhs.value.as < Inode::Inode_t* > () = Inode::make_bin_op(yystack_[2].value.as < Inode::Inode_t* > (), Inode::bin_op::mult_, yystack_[0].value.as < Inode::Inode_t* > ());}
#line 897 "compiler.tab.cc"
    break;

  case 18: // expr: expr DIV expr
#line 80 "compiler.y"
                                                {yylhs.value.as < Inode::Inode_t* > () = Inode::make_bin_op(yystack_[2].value.as < Inode::Inode_t* > (), Inode::bin_op::div_, yystack_[0].value.as < Inode::Inode_t* > ());}
#line 903 "compiler.tab.cc"
    break;

  case 19: // expr: expr MOD expr
#line 81 "compiler.y"
                                                {yylhs.value.as < Inode::Inode_t* > () = Inode::make_bin_op(yystack_[2].value.as < Inode::Inode_t* > (), Inode::bin_op::mod_, yystack_[0].value.as < Inode::Inode_t* > ());}
#line 909 "compiler.tab.cc"
    break;

  case 20: // expr: expr AND expr
#line 82 "compiler.y"
                                                {yylhs.value.as < Inode::Inode_t* > () = Inode::make_bin_op(yystack_[2].value.as < Inode::Inode_t* > (), Inode::bin_op::and_, yystack_[0].value.as < Inode::Inode_t* > ());}
#line 915 "compiler.tab.cc"
    break;

  case 21: // expr: expr OR expr
#line 83 "compiler.y"
                                                {yylhs.value.as < Inode::Inode_t* > () = Inode::make_bin_op(yystack_[2].value.as < Inode::Inode_t* > (), Inode::bin_op::or_, yystack_[0].value.as < Inode::Inode_t* > ());}
#line 921 "compiler.tab.cc"
    break;

  case 22: // expr: NOT expr
#line 84 "compiler.y"
                                                {yylhs.value.as < Inode::Inode_t* > () = Inode::make_unary_op(yystack_[0].value.as < Inode::Inode_t* > (), Inode::unary_op::not_);}
#line 927 "compiler.tab.cc"
    break;

  case 23: // expr: expr EQUAL expr
#line 85 "compiler.y"
                                                {yylhs.value.as < Inode::Inode_t* > () = Inode::make_bin_op(yystack_[2].value.as < Inode::Inode_t* > (), Inode::bin_op::equal_, yystack_[0].value.as < Inode::Inode_t* > ());}
#line 933 "compiler.tab.cc"
    break;

  case 24: // expr: expr NOT_EQUAL expr
#line 86 "compiler.y"
                                                {yylhs.value.as < Inode::Inode_t* > () = Inode::make_bin_op(yystack_[2].value.as < Inode::Inode_t* > (), Inode::bin_op::not_equal_, yystack_[0].value.as < Inode::Inode_t* > ());}
#line 939 "compiler.tab.cc"
    break;

  case 25: // expr: expr GREATER expr
#line 87 "compiler.y"
                                                {yylhs.value.as < Inode::Inode_t* > () = Inode::make_bin_op(yystack_[2].value.as < Inode::Inode_t* > (), Inode::bin_op::greater_, yystack_[0].value.as < Inode::Inode_t* > ());}
#line 945 "compiler.tab.cc"
    break;

  case 26: // expr: expr LESS expr
#line 88 "compiler.y"
                                                {yylhs.value.as < Inode::Inode_t* > () = Inode::make_bin_op(yystack_[2].value.as < Inode::Inode_t* > (), Inode::bin_op::less_, yystack_[0].value.as < Inode::Inode_t* > ());}
#line 951 "compiler.tab.cc"
    break;

  case 27: // expr: expr GREATER_OR_EQUAL expr
#line 89 "compiler.y"
                                                {yylhs.value.as < Inode::Inode_t* > () = Inode::make_bin_op(yystack_[2].value.as < Inode::Inode_t* > (), Inode::bin_op::greater_or_equal_, yystack_[0].value.as < Inode::Inode_t* > ());}
#line 957 "compiler.tab.cc"
    break;

  case 28: // expr: expr LESS_OR_EQUAL expr
#line 90 "compiler.y"
                                                {yylhs.value.as < Inode::Inode_t* > () = Inode::make_bin_op(yystack_[2].value.as < Inode::Inode_t* > (), Inode::bin_op::less_or_equal_, yystack_[0].value.as < Inode::Inode_t* > ());}
#line 963 "compiler.tab.cc"
    break;

  case 29: // expr: NAME
#line 91 "compiler.y"
                                                {yylhs.value.as < Inode::Inode_t* > () = current_scope->visible(yystack_[0].value.as < std::string > ());}
#line 969 "compiler.tab.cc"
    break;

  case 30: // expr: INTEGER
#line 92 "compiler.y"
                                                {yylhs.value.as < Inode::Inode_t* > () = Inode::make_value(yystack_[0].value.as < int > ());}
#line 975 "compiler.tab.cc"
    break;

  case 31: // expr: INPUT
#line 93 "compiler.y"
                                                {yylhs.value.as < Inode::Inode_t* > () = Inode::make_unary_op(Inode::unary_op::input_, nullptr);}
#line 981 "compiler.tab.cc"
    break;

  case 32: // if: IF LRB expr RRB scope
#line 95 "compiler.y"
                                                {yylhs.value.as < Inode::Inode_t* > () = Inode::make_if(yystack_[2].value.as < Inode::Inode_t* > (), yystack_[0].value.as < Inode::Iscope_t* > ());}
#line 987 "compiler.tab.cc"
    break;

  case 33: // while: WHILE LRB expr RRB scope
#line 97 "compiler.y"
                                                {yylhs.value.as < Inode::Inode_t* > () = Inode::make_while(yystack_[2].value.as < Inode::Inode_t* > (), yystack_[0].value.as < Inode::Iscope_t* > ());}
#line 993 "compiler.tab.cc"
    break;

  case 34: // output: OUTPUT expr SCOLON
#line 99 "compiler.y"
                                                {yylhs.value.as < Inode::Inode_t* > () = Inode::make_unary_op(Inode::unary_op::output_, yystack_[1].value.as < Inode::Inode_t* > ());}
#line 999 "compiler.tab.cc"
    break;


#line 1003 "compiler.tab.cc"

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
  "MOD", "$accept", "scope", "close_sc", "stm", "assign", "lval", "if",
  "while", "expr", "output", "stms", "open_sc", "program", YY_NULLPTR
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




  const signed char parser::yypact_ninf_ = -13;

  const signed char parser::yytable_ninf_ = -1;

  const signed char
  parser::yypact_[] =
  {
      51,   -13,    -5,     1,     0,   -13,   -13,     2,   -13,   -13,
     -13,    29,    36,     0,     0,   -13,   -13,   -13,     0,    24,
       0,   -13,   -13,   -13,    51,   -13,    64,    82,   106,   -13,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    46,     4,    30,    30,    95,   106,   115,
     115,   120,   120,   120,   120,    32,    32,   -13,   -13,   -13,
     -13,   -13,   -13,   -13,   -13
  };

  const signed char
  parser::yydefact_[] =
  {
       0,    14,     0,     0,     0,     6,     9,     0,    10,    11,
      12,     2,     0,     0,     0,    29,    30,    31,     0,     0,
       0,     4,     8,     7,     0,     1,     0,     0,    22,    34,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    21,    20,    23,
      24,    25,    26,    28,    27,    15,    16,    17,    18,    19,
      13,     5,     3,    33,    32
  };

  const signed char
  parser::yypgoto_[] =
  {
     -13,    31,   -13,   -11,   -13,   -13,   -13,   -13,   -12,   -13,
      54,   -13,   -13
  };

  const signed char
  parser::yydefgoto_[] =
  {
      -1,    22,    62,     5,     6,     7,     8,     9,    19,    10,
      11,    24,    12
  };

  const signed char
  parser::yytable_[] =
  {
      23,    26,    27,    15,    16,    13,    28,     1,    43,     2,
       3,    14,    21,    61,    17,    20,     4,    18,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    29,     1,    23,     2,     3,    25,    21,    21,    30,
      31,     4,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    60,     1,     0,     2,     3,    40,    41,
      42,    30,    31,     4,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    45,    63,    64,    44,    30,
      31,     0,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    46,     0,     0,     0,    30,    31,     0,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    31,     0,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    38,    39,    40,    41,    42
  };

  const signed char
  parser::yycheck_[] =
  {
      11,    13,    14,     3,     4,    10,    18,     3,    20,     5,
       6,    10,     8,     9,    14,    13,    12,    17,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,     7,     3,    44,     5,     6,     0,     8,     8,    15,
      16,    12,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,     7,     3,    -1,     5,     6,    26,    27,
      28,    15,    16,    12,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    11,    45,    46,    24,    15,
      16,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    11,    -1,    -1,    -1,    15,    16,    -1,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    16,    -1,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    24,    25,    26,    27,    28
  };

  const signed char
  parser::yystos_[] =
  {
       0,     3,     5,     6,    12,    32,    33,    34,    35,    36,
      38,    39,    41,    10,    10,     3,     4,    14,    17,    37,
      13,     8,    30,    32,    40,     0,    37,    37,    37,     7,
      15,    16,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    37,    39,    11,    11,    37,    37,    37,
      37,    37,    37,    37,    37,    37,    37,    37,    37,    37,
       7,     9,    31,    30,    30
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    29,    41,    30,    40,    31,    39,    39,    39,    32,
      32,    32,    32,    33,    34,    37,    37,    37,    37,    37,
      37,    37,    37,    37,    37,    37,    37,    37,    37,    37,
      37,    37,    35,    36,    38
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     1,     3,     1,     1,     1,     2,     2,     1,
       1,     1,     1,     4,     1,     3,     3,     3,     3,     3,
       3,     3,     2,     3,     3,     3,     3,     3,     3,     1,
       1,     1,     5,     5,     3
  };




#if YYDEBUG
  const signed char
  parser::yyrline_[] =
  {
       0,    51,    51,    53,    55,    59,    64,    65,    66,    68,
      69,    70,    71,    73,    75,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    95,    97,    99
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
      return YY_CAST (symbol_kind_type, translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

} // yy
#line 1443 "compiler.tab.cc"

#line 101 "compiler.y"


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
