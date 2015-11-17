%option c++ yyclass="pwn_scanner" outfile="pwn_scanner.cpp" debug
%option stack noyywrap yylineno 8bit
%{
/* $Id: pwn_scanner.l,v 1.18 2015/05/19 15:10:48 ist175741 Exp $ */
// make relevant includes before including the parser's tab file
#include <string>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include "pwn_scanner.h"
#include "pwn_parser.tab.h"

// don't change this
#define yyerror LexerError

%}
INT		([-]?0|[1-9][0-9]*)
INT16		([-]?"0x"(0|[1-9a-fA-F][0-9a-fA-F]*))
DOUBLE 		([0-9]+"."[0-9]+"e""-"?[0-9]+)|([0-9]+"."[0-9]+)|([0-9]+"e""-"?[0-9]+)
IDENTIFIER 	[A-Za-z_][A-Za-z0-9_]*
SPECIAL		[-()<>=+*/%;{}~|,.#!?&$\[\]@]
HEX			[A-Fa-f0-9]

%x X_STRING X_COMMENT X_STRING_DC
%% { set_debug(1); }

"//".*			; /* ignore comments */ /* !!! FIX ME: $ */
"/*"			yy_push_state(X_COMMENT);
<X_COMMENT>"*/"		yy_pop_state();
<X_COMMENT>"/*"		yy_push_state(X_COMMENT);
<X_COMMENT>.|\n		;

">="			return tGE;
"<="			return tLE;
"=="			return tEQ;
"<>"			return tNE;
"!!"			return tPLN;


"repeat"		return tREPEAT;
"next"			return tNEXT;
"stop"			return tSTOP;
"if"			return tIF;
"else"			return tELSE;
"print"			return tPRINT;
"@"			return tREAD;
"local"			return tLOCAL;
"import"		return tIMPORT;
"return"		return tRETURN;
"noob"			return tNOOB;

{IDENTIFIER}			yylval.s = new std::string(yytext); return tIDENTIFIER;

\"				yy_push_state(X_STRING); yylval.s = new std::string("");
<X_STRING>\\\\			*yylval.s += yytext + 1;
<X_STRING>\\n			*yylval.s += "\n";
<X_STRING>\\r			*yylval.s += "\r";
<X_STRING>\\t			*yylval.s += "\t";
<X_STRING>\\\"			*yylval.s += "\"";

<X_STRING>\\{HEX}{HEX}?		{ int aux = strtol(yytext+1, nullptr, 16); *yylval.s += (char) aux;}

<X_STRING>\"			{yy_pop_state(); return tSTRING;  }
<X_STRING>.			*yylval.s += yytext;

{DOUBLE}			yylval.d = strtod(yytext, nullptr); return tDOUBLE;
{INT16}				yylval.i = strtol(yytext, nullptr, 16); return tINTEGER;
{INT}				yylval.i = strtol(yytext, nullptr, 10); return tINTEGER;

{SPECIAL}			return *yytext;

[ \t\n]+			; /* ignore whitespace */

.				std::cout << "[C]" << yytext << "[/C]" << std::endl; yyerror("Unknown character");


%%

// Very, very dirty hack: flex is a mess generating C++ scanners.
int pwn_scanner::yywrap() { return 1; }
