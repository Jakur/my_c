%option interactive
%{
/* * * * * * * * * * * *
 * * * DEFINITIONS * * *
 * * * * * * * * * * * */
%}

%{

// y.tab.h contains the token number values produced by the parser
#include <string.h>
#include "my-c.h"
#include "y.tab.h"

extern int line_num;

%}


%option noyywrap
DIGIT [0-9]
LETTER [a-zA-Z]

%{ 
  /* * * * * * * * * *
   * * * STATES  * * *
   * * * * * * * * * */
%}

%x ERROR

%%

%{
/* * * * * * * * * 
 * * * RULES * * *
 * * * * * * * * */
%}

print   { return PRINT; }
{DIGIT}+ {
	yylval.number = atof(yytext); return NUMBER;
	}

{LETTER}[0-9a-zA-Z]* {
        yylval.var_name = strdup(yytext); return ID;
	}

[ \t\f\r]	;		 // ignore white space 

\n      { line_num++; }
"-"	{ return MINUS;  }
"+"	{ return PLUS;   }
"*"	{ return TIMES;  }
"/"	{ return DIVIDE; }
"("	{ return LPAREN; }
")"	{ return RPAREN; }
"{"	{ return LBRACE; }
"}"	{ return RBRACE; }
"="     { return EQUALS; }
";"    {return SEMICOLON;}

"&&" {return AND;}

"pass" {return PASS;} // PASS RETURN IF THEN ELSE END WHILE DO
"return" {return RETURN;}
"if" {return IF;}
"then" {return THEN;}
"else" {return ELSE;}
"end" {return END;}
"while" {return WHILE;}
"do" {return DO;}
"," {return COMMA;}

. { BEGIN(ERROR); yymore(); }
<ERROR>[^{DIGIT}{LETTER}+\-/*(){}= \t\n\f\r] { yymore(); }
<ERROR>(.|\n) { yyless(yyleng-1); printf("error token: %s on line %d\n", yytext, line_num); 
           BEGIN(INITIAL); }

%%

