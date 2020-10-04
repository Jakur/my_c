%{


/* This interpreter evaluates arithmetic expressions and assigns
   them to the specified variable names. The grammar is:

   pgm -> stmtlist
   stmtlist -> stmt | stmtlist ; stmt
   stmt -> id = exp 
        |  print id
   exp -> exp + mulexp | exp - mulexp 
   mulexp -> mulexp * primexp | mulexp / primexp
   primexp ->  ( exp ) | ( exp ) | - primexp | id | number 
*/

#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <list>
#include <optional>
#include "my-c.h"



using namespace std;

 extern FILE *yyin;

// the root of the abstract syntax tree
 stmt_node *root;
 Fn *main_fn;

// for keeping track of line numbers in the program we are parsing
  int line_num = 1;

// function prototypes, we need the yylex return prototype so C++ won't complain
int yylex();
void yyerror(const char* s);

%}

%start program

%union {
  float number;
  int integer;
  char * var_name;
  Exp *exp_node_ptr;
  stmt_node *stmt_node_ptr;
  Stmt *stmt_ptr;
  Fn *fn;
}

%error-verbose

%token <number> NUMBER
%token <integer> INTEGER
%token <number> FLOAT
%token <var_name> ID
%token SEMICOLON EQUALS PRINT PLUS MINUS TIMES DIVIDE LPAREN RPAREN LBRACE RBRACE LSQUARE RSQUARE
%token AND
%token PASS RETURN IF THEN ELSE END WHILE DO COMMA
%token CHAR BOOL STRING EXCLAM
%token T_INT T_FLOAT
// New
%type <stmt_ptr> stmts stmt var_dec asgn_stmt if_stmt while_stmt return_stmt print_stmt
%type <exp_node_ptr> exp weak_exp strong_exp not_exp num_term func_invo array_index bool_term
%type <fn> func_decl
%%
// New
program:
    func_decl {if (main_fn == nullptr) {main_fn = $1;}}
;
stmts:
    stmt
  | stmt stmts
;
stmt:
    PASS SEMICOLON {new Pass();}
  | var_dec
  | asgn_stmt
  | if_stmt 
  | while_stmt 
  | return_stmt
  | print_stmt 
;

// Need some way to output
print_stmt: 
  PRINT exp SEMICOLON {$$ = new print_node($02);}

asgn_stmt:
  ID EQUALS exp SEMICOLON {$$ = new assign_node($1, $3);}
;
// Types 
prim: 
  T_INT
  | T_FLOAT
;
type: 
  prim {}
  | array_type {}
;

dims: 
    LSQUARE exp RSQUARE
  | LSQUARE exp RSQUARE dims
;
array_type: 
  prim dims
;
array_index:
  ID dims {new LiteralExp(Data(0));}
;

// Control Flow
if_stmt: 
  IF exp THEN stmts END {new Pass();}
  | IF exp THEN stmts ELSE stmts END {new Pass();}
;
while_stmt:
  WHILE exp DO stmts END {new Pass();}
;

// Operators and expressions
exp: 
  bool_term 
  | exp bool_op bool_term {new BinaryExp($01, BinaryOperator::ADD, $03);}
;
bool_op:
  AND
;
bool_term:
  weak_exp 
  | bool_term weak_exp {new LiteralExp(0);}
;

weak_exp:
  strong_exp
  | weak_exp weak_op strong_exp {new BinaryExp($01, BinaryOperator::ADD, $03);}
;

weak_op:
  PLUS
  | MINUS
;

strong_exp:
  not_exp 
  | strong_exp strong_op not_exp {new BinaryExp($01, BinaryOperator::ADD, $03);}
;

strong_op:
  TIMES
  | DIVIDE
;

not_exp:
  EXCLAM num_term {new LiteralExp(Data(0));}
  | num_term
;

num_term:
  func_invo 
  | array_index
  | LPAREN exp RPAREN {$$ = $2;}
  | ID {$$ = 0;}
  | INTEGER {$$ = new LiteralExp(Data($1));}
;

// Var Dec 
var_dec:
  type declaration_list SEMICOLON {$$ = new Pass();}
  | type ID EQUALS LBRACE exp_list RBRACE SEMICOLON {$$ = new Pass();}
;
declaration_list:
  declaration 
  | declaration COMMA declaration_list
;
declaration:
  ID
  | ID EQUALS exp
;

// Functions
func_decl:
  type ID LPAREN RPAREN stmts END {$$ = new Fn($05);}
  | type ID LPAREN parameter_list RPAREN stmts END {} {$$ = new Fn($06);}
;
parameter_list:
  type ID {} 
  | type ID COMMA parameter_list {}
;
func_invo:
  ID LPAREN RPAREN {}
  | ID LPAREN exp_list RPAREN {}
;
exp_list:
  exp {}
  | exp_list COMMA exp {}
;
return_stmt:
  RETURN ID SEMICOLON {}
;


// Old
// program: stmtlist { root = $$; }
// ;



// stmtlist: stmtlist SEMICOLON stmt
//             { // copy up the list and add the stmt to it
//               $$ = new sequence_node($1,$3);
//             }
//          | stmtlist SEMICOLON error
// 	   { // just copy up the stmtlist when an error occurs
//              $$ = $1;
//              yyclearin; } 
//          |  stmt 
// 	 { $$ = $1;   }
// ;

// stmt: ID EQUALS exp { 
//   $$ = new assign_node($1, $3);
// 	   }
       
// | PRINT exp {
//   $$ = new print_node($2);
//  }

// |
// { $$ = new skip_node();
// }
// | LBRACE stmtlist RBRACE { $$=$2; } 
//  ;


// exp:	exp PLUS mulexp { $$ = new BinaryExp($1, BinaryOperator::ADD, $3); }

//       |	exp MINUS mulexp { $$ = new BinaryExp($1, BinaryOperator::SUB, $3); }

//       |	mulexp {  $$ = $1; }
// ;



// mulexp:	mulexp TIMES primexp {
// 	  $$ = new BinaryExp($1, BinaryOperator::MUL, $3); }

//       | mulexp DIVIDE primexp {
// 	  $$ = new BinaryExp($1, BinaryOperator::DIV, $3); }

//       | primexp { $$=$1;  }
// ;


// // Fix negation
// primexp:	LPAREN exp RPAREN  {  $$ = $2; }

//       |	NUMBER { $$ = new LiteralExp(Data($1)); }

//       | ID { $$ = new VarExp($1); }
// ;
 
%%
int main(int argc, char **argv)
{ 
  if (argc>1) yyin=fopen(argv[1],"r");

  //  yydebug = 1;
  yyparse();

  cout << "---------- list of input program------------" << endl << endl;
  // root -> print();

  cout << "---------- execution of input program------------" << endl << endl;
  if (main_fn == nullptr) {
    cout << "NULL MAIN" << endl;
  } else {
    cout << "LENGTH: " << main_fn->stmts.size() << endl;
    main_fn->fn_call();
  }

  // root->evaluate();
  cout << "TESTING" << endl;
}

void yyerror(const char * s)
{
  fprintf(stderr, "line %d: %s\n", line_num, s);
}

