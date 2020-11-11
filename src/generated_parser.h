/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_GENERATED_PARSER_H_INCLUDED
# define YY_YY_GENERATED_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 10 "parser.y" /* yacc.c:1909  */

#include <string>

#line 48 "generated_parser.h" /* yacc.c:1909  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TYPEDEF = 258,
    EXTERN = 259,
    STATIC = 260,
    AUTO = 261,
    REGISTER = 262,
    VOID = 263,
    CHAR = 264,
    SHORT = 265,
    INT = 266,
    LONG = 267,
    FLOAT = 268,
    DOUBLE = 269,
    SIGNED = 270,
    UNSIGNED = 271,
    CONST = 272,
    RESTRICT = 273,
    VOLATILE = 274,
    INLINE = 275,
    SIZEOF = 276,
    ENUM = 277,
    STRUCT = 278,
    UNION = 279,
    BREAK = 280,
    CASE = 281,
    CONTINUE = 282,
    DEFAULT = 283,
    DO = 284,
    ELSE = 285,
    FOR = 286,
    GOTO = 287,
    IF = 288,
    RETURN = 289,
    SWITCH = 290,
    WHILE = 291,
    ASSIGN = 292,
    STARASS = 293,
    OVERASS = 294,
    MODASS = 295,
    PLUSASS = 296,
    MINASS = 297,
    SLASS = 298,
    SRASS = 299,
    ANDASS = 300,
    XORASS = 301,
    ORASS = 302,
    PLUS = 303,
    MINUS = 304,
    STAR = 305,
    OVER = 306,
    PERC = 307,
    PLUSPLUS = 308,
    MINUSMINUS = 309,
    EQ = 310,
    NEQ = 311,
    LT = 312,
    GT = 313,
    LET = 314,
    GET = 315,
    AND = 316,
    OR = 317,
    BTOR = 318,
    BTXOR = 319,
    NOT = 320,
    BTNOT = 321,
    LEFT = 322,
    RIGHT = 323,
    LPAR = 324,
    RPAR = 325,
    LCB = 326,
    RCB = 327,
    LB = 328,
    RB = 329,
    DOT = 330,
    ARROW = 331,
    AMPER = 332,
    SEMI = 333,
    COLON = 334,
    COMMA = 335,
    QUEST = 336,
    ELLIPSIS = 337,
    INT_VAL = 338,
    REAL_VAL = 339,
    CHAR_VAL = 340,
    STR_VAL = 341,
    ID = 342,
    TYPENAME = 343
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef struct YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_GENERATED_PARSER_H_INCLUDED  */
