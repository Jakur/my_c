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
void analyzer();
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
  DataType *type_info;
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
%token T_INT T_FLOAT T_BOOL T_CHAR T_STRING T_ARRAY
// New
%type <type_info> prim array_type type
%type <integer> bool_op weak_op strong_op
%type <stmt_ptr> stmt var_dec asgn_stmt if_stmt while_stmt return_stmt print_stmt declaration
%type <multi_stmt> stmts declaration_list
%type <exp_list> exp_list dims
%type <exp_node_ptr> exp weak_exp strong_exp not_exp num_term func_invo array_index
%type <fn> func_decl
%type <parms> parameter_list
%%
// New
program:
    func_decl {auto f = $01; fns[f->ident] = f;}
    | func_decl program {
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
  T_INT {$$ = new DataType(Data(0));}
  | T_FLOAT {$$ = new DataType(Data(0.0f));}
  | T_BOOL {$$ = new DataType(Data(false));}
  | T_CHAR {$$ = new DataType(Data('a'));}
  | T_STRING {$$ = new DataType(Data(""));}
;
type: 
  prim 
  | array_type
;

dims: 
    LSQUARE exp RSQUARE {$$ = new ExpList($02);}
  | LSQUARE exp RSQUARE dims {
    ExpList *s = $04;
    auto next = $02;
    s->exps.push_back(next);
    $$ = s;
  }
;
array_type: 
  prim dims {
    auto init = $01;
    ExpList *s = $02;
    Data d = Data(&(init->init));
    $$ = new DataType(d, s);
    }
;
array_index:
  ID dims {$$ = new IndexExp($01, $02);}
;

// Control Flow
if_stmt: 
  IF exp THEN stmts END {$$ = new IfStmt($02, $04, nullptr);}
  | IF exp THEN stmts ELSE stmts END {$$ = new IfStmt($02, $04, $06);}
;
while_stmt:
  WHILE exp DO stmts END {$$ = new WhileStmt($02, $04);}
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
  | type ID EQUALS LBRACE exp_list RBRACE SEMICOLON {
    DataType *d = $01;
    ExpList *e = $05;
    ArrayInitExp *a = new ArrayInitExp(d, e);
    $$ = new AssignStmt($02, a);
  }
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
  ID {$$ = new DeclareStmt($01);}
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
  
  yyparse();
  auto g = new FlowGraph();

  cout << "---------- List of Input Program------------" << endl << endl;
  map<string, Fn *>::iterator it;
  for (it = fns.begin(); it != fns.end(); it++) {
    std::cout << "Fn " << it->first << ": " << endl;
    it->second->print();
    it->second->stmts->compute_flow(g, -1, -1);
  }
  main_fn = fns["main"];
  cout << "---------- Flow Graph Adjacency Lists------------" << endl << endl;
  g->print_edges();
  cout << "---------- Reaching Defintions------------" << endl << endl;
  analyzer();

  cout << "---------- Execution of Input Program------------" << endl << endl;
  if (main_fn == nullptr) {
    cout << "NULL MAIN" << endl;
  } else {
    main_fn->fn_call(VarStorage(map<string, Data>()));
  }
}

void analyzer() {
  for (auto it = fns.begin(); it != fns.end(); it++) {
    auto g = new FlowGraph();
    it->second->stmts->compute_flow(g, -1, -1);
    auto rds = map<int, ReachingDefinition>();
    std::cout << "Fn " << it->first << " Reaching Definitions: " << endl << endl;
    auto stmts = it->second->stmts->stmts;
    int start = stmts[stmts.size() - 1]->label();
    auto parms = it->second->parameters;
    if (parms.size() == 0) {
      cout << "RD_Entry(" << start << "): { }" << endl;
    } else {
      cout << "RD_Entry(" << start << "): {";
      for (auto p : parms) {
        cout << "( " << p << ", ?)";
      }
      cout << "}" << endl;
    }
    // Output Entry Equations
    for (auto pair : g->rev_edges) {
      int key = pair.first;
      if (key == start) {
        continue; // Handled above
      }
      auto set = pair.second;
      cout << "RD_Entry(" << key << ") = ";
      int size = set.size();
      if (size == 0) {
        cout << "{ }" << endl;
      } else {
        int counter = 0;
        for (auto element : set) {
          counter += 1;
          cout << "RD_Exit(" << element << ")";
          if (counter < size) {
            cout << " U ";
          }
        }
        cout << endl;
      }
    }
    cout << endl;
    // Output Exit Equations
    for (auto pair : g->nodes) {
      int label = pair.first;
      auto node = pair.second;
      auto kill_var = node->kill_set();
      auto gen_var = node->gen_set();
      std::set<int> entries = g->rev_edges[label];
      cout << "RD_Exit(" << label << ") = ";
      if (kill_var.has_value() && gen_var.has_value()) {
        cout << "(RD_Entry(" << label << ") \\ {(" << kill_var.value() << ", ANY)}"
          << ") U " << "(" << gen_var.value() << ", " << label << ")" << endl;
        auto rd = ReachingDefinition(label, std::set<int>(entries), gen_var.value());
        rds.insert({label, rd});

      } else {
        cout << "RD_Entry(" << label << ")" << endl;
        auto rd = ReachingDefinition(label, std::set<int>(entries));
        rds.insert({label, rd});
      }
    
    }
    cout << endl;
  }
}

void worklist_algorithm(FlowGraph *g, map<int, ReachingDefinition> &solutions) {
  auto worklist = list<int>();
  // Initialize the worklist with all labels in the flow
  for (auto pair : g->nodes) {
    int node_label = pair.first;
    worklist.push_back(node_label);
  }
  while (worklist.size() > 0) {
    break;
  }
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

