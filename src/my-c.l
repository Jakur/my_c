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
{DIGIT}+[.]{DIGIT}* {
	yylval.number = atof(yytext); return FLOAT;
	}
{DIGIT}+ {
        yylval.integer = atoi(yytext); return INTEGER;
}
"true" {yylval.boolean = true; return BOOL;}
"false" {yylval.boolean = false; return BOOL;}
['][0-9a-zA-Z]['] {yylval.character = yytext[0]; return CHAR;}


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
"["     {return LSQUARE;}
"]"     {return RSQUARE;}
"="     { return EQUALS; }
";"    {return SEMICOLON;}

"&&" {return AND;}
"||" {return OR;}
"int" {return T_INT;}
"float" {return T_FLOAT;}
"bool" {return T_BOOL;}
"char" {return T_CHAR;}
"string" {return T_STRING;}

"pass" {return PASS;} // PASS RETURN IF THEN ELSE END WHILE DO
"return" {return RETURN;}
"if" {return IF;}
"then" {return THEN;}
"else" {return ELSE;}
"end" {return END;}
"while" {return WHILE;}
"do" {return DO;}
"," {return COMMA;}
"!" {return EXCLAM;}
"end" {return END;}

{LETTER}[0-9a-zA-Z]* {
        yylval.var_name = strdup(yytext); return ID;
	}

. { BEGIN(ERROR); yymore(); }
<ERROR>[^{DIGIT}{LETTER}+\-/*(){}= \t\n\f\r] { yymore(); }
<ERROR>(.|\n) { yyless(yyleng-1); printf("error token: %s on line %d\n", yytext, line_num); 
           BEGIN(INITIAL); }

%%

