%{


#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <list>
#include "my-c.h"



using namespace std;

 extern FILE *yyin;

// the entry point of execution
 Fn *main_fn;

// for keeping track of line numbers in the program we are parsing
  int line_num = 1;

// function prototypes, we need the yylex return prototype so C++ won't complain
int yylex();
void yyerror(const char* s);
BinaryOperator get_op(int x);

%}

%start program

%union {
  float number;
  int integer;
  bool boolean;
  char character;
  char *chars;
  std::string *str;
  Exp *exp_node_ptr;
  Stmt *stmt_ptr;
  ExpList *exp_list;
  MultiStmt *multi_stmt;
  ParmList *parms;
  Fn *fn;
}

// %define parse.error verbose

%token <integer> INTEGER
%token <number> FLOAT
%token <boolean> BOOL
%token <character> CHAR
%token <str> STRING
%token <chars> ID
%token PLUS MINUS TIMES DIVIDE AND OR 
%token SEMICOLON EQUALS PRINT LPAREN RPAREN LBRACE RBRACE LSQUARE RSQUARE
%token LESSTHAN LESSTHANE GREATTHAN GREATTHANE NOTEQUAL EQUALTO;
%token PASS RETURN IF THEN ELSE END WHILE DO COMMA EXCLAM
%token T_INT T_FLOAT T_BOOL T_CHAR T_STRING
%token PCT
// New
%type <integer> bool_op weak_op strong_op type prim
%type <stmt_ptr> stmt var_dec asgn_stmt if_stmt while_stmt return_stmt print_stmt declaration
%type <multi_stmt> stmts declaration_list
%type <exp_list> exp_list
%type <exp_node_ptr> exp weak_exp strong_exp not_exp num_term func_invo array_index
%type <fn> func_decl
%type <parms> parameter_list
%%
// New
program:
    func_decl {auto f = $01; fns[f->ident] = f; cout << "b";}
    | func_decl program {
      cout << "a";
      auto f = $01; fns[f->ident] = f;
    }
;
stmts:
    stmt {$$ = new MultiStmt($01);}
  | stmt stmts {
    MultiStmt *s = $02;
    auto next = $01;
    s->stmts.push_back(next);
    $$ = s;
  }
;
stmt:
    PASS SEMICOLON {$$ = new Pass();}
  | var_dec
  | asgn_stmt
  | if_stmt 
  | while_stmt 
  | return_stmt
  | print_stmt 
;

// Need some way to output
print_stmt: 
  PRINT exp SEMICOLON {$$ = new PrintStmt($02);}

asgn_stmt:
  ID EQUALS exp SEMICOLON {$$ = new AssignStmt($01, $03);}
;
// Types 
prim: 
  T_INT {$$ = T_INT;}
  | T_FLOAT {$$ = T_FLOAT;}
  | T_BOOL {$$ = T_BOOL;}
  | T_CHAR {$$ = T_CHAR;}
  | T_STRING {$$ = T_STRING;}
;
type: 
  prim 
  | array_type {$$ = T_INT;} // TODO
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
  IF exp THEN stmts END {$$ = new IfStmt($02, $04, nullptr);}
  | IF exp THEN stmts ELSE stmts END {$$ = new IfStmt($02, $04, $06);}
;
while_stmt:
  WHILE exp DO stmts END {$$ = new Pass();}
;

// Operators and expressions
exp: 
  weak_exp 
  | exp bool_op weak_exp {$$ = new BinaryExp($01, get_op($02), $03);}
;
bool_op:
  AND {$$ = AND;}
  | OR {$$ = OR;}
  | LESSTHAN {$$ = LESSTHAN;}
  | LESSTHANE {$$ = LESSTHANE;}
  | GREATTHAN {$$ = GREATTHAN;}
  | GREATTHANE {$$ = GREATTHANE;}
  | NOTEQUAL {$$ = NOTEQUAL;}
  | EQUALTO {$$ = EQUALTO;}
;
weak_exp:
  strong_exp
  | weak_exp weak_op strong_exp {$$ = new BinaryExp($01, get_op($02), $03);}
;

weak_op:
  PLUS {$$ = PLUS;}
  | MINUS {$$ = MINUS;}
;

strong_exp:
  not_exp
  | strong_exp strong_op not_exp {$$ = new BinaryExp($01, get_op($02), $03);}
;

strong_op:
  TIMES {$$ = TIMES;}
  | DIVIDE {$$ = DIVIDE;}
;

not_exp:
  EXCLAM num_term {$$ = new NegationExp($02);}
  | num_term
;

num_term:
  func_invo 
  | array_index
  | LPAREN exp RPAREN {$$ = $02;}
  | ID {$$ = new VarExp($01);}
  | INTEGER {
    int x = $01;
    $$ = new LiteralExp(Data(x));
    }
  | FLOAT {
    float f = $01;
    $$ = new LiteralExp(Data(f));
  }
  | BOOL {
    bool b = $01;
    $$ = new LiteralExp(Data(b));
  }
  | CHAR {
    char c = $01;
    $$ = new LiteralExp(Data(c));
  }
  | STRING {
    std::string *s = $01;
    $$ = new LiteralExp(Data(s));
  }
;

// Var Dec 
var_dec:
  type declaration_list SEMICOLON {$$ = $02;}
  | type ID EQUALS LBRACE exp_list RBRACE SEMICOLON {$$ = new Pass();}
; 
declaration_list:
  declaration {$$ = new MultiStmt($01);}
  | declaration COMMA declaration_list {
    MultiStmt *s = $03;
    auto next = $01;
    s->stmts.push_back(next);
    $$ = s;
  }
;
declaration:
  ID {$$ = new Pass();}
  | ID EQUALS exp {$$ = new AssignStmt($01, $03);}
;

// Functions
func_decl:
  type ID LPAREN RPAREN stmts END {$$ = new Fn($02, std::vector<std::string>(), $05);}
  | type ID LPAREN parameter_list RPAREN stmts END {} {auto p = $04; $$ = new Fn($02, p->names, $06);}
;
parameter_list:
  type ID {$$ = new ParmList($02);} 
  | type ID COMMA parameter_list {
    ParmList *s = $04;
    auto next = $02;
    s->names.push_back(next);
    $$ = s;
  }
;
func_invo:
  ID LPAREN RPAREN {$$ = new FnCallExp($01, nullptr);}
  | ID LPAREN exp_list RPAREN {$$ = new FnCallExp($01, $03);}
;
exp_list:
  exp {$$ = new ExpList($01);}
  | exp COMMA exp_list {
    ExpList *s = $03;
    auto next = $01;
    s->exps.push_back(next);
    $$ = s;
  }
;
return_stmt:
  RETURN exp SEMICOLON {$$ = new ReturnStmt($02);}
;

%%
int main(int argc, char **argv)
{ 
  if (argc>1) yyin=fopen(argv[1],"r");

  //  yydebug = 1;
  yyparse();

  cout << "---------- list of input program------------" << endl << endl;
  // root -> print();
  cout << "Size: " << fns.size() << endl;
  main_fn = fns["main"];
  cout << "---------- execution of input program------------" << endl << endl;
  if (main_fn == nullptr) {
    cout << "NULL MAIN" << endl;
  } else {
    cout << "main() statement count: " << main_fn->stmts->stmts.size() << endl;
    main_fn->fn_call(VarStorage(map<string, Data>()));
  }

  cout << "Finished Testing" << endl;
}

BinaryOperator get_op(int x) {
  switch (x) {
    case MINUS: return BinaryOperator::SUB;
    case PLUS: return BinaryOperator::ADD;
    case TIMES: return BinaryOperator::MUL;
    case DIVIDE: return BinaryOperator::DIV;
    case AND: return BinaryOperator::BAND;
    case OR: return BinaryOperator::BOR;
    case LESSTHAN: return BinaryOperator::LTHAN;
    case LESSTHANE: return BinaryOperator::LETHAN;
    case GREATTHAN: return BinaryOperator::GTHAN;
    case GREATTHANE: return BinaryOperator::GETHAN;
    case NOTEQUAL: return BinaryOperator::NEQ;
    case EQUALTO: return BinaryOperator::EQ;
  }
  cout << "Unknown binary operator with number " << x << endl;
  return BinaryOperator::ADD; 
}

void yyerror(const char * s)
{
  fprintf(stderr, "line %d: %s\n", line_num, s);
}

