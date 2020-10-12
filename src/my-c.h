#include <iostream>
#include <stdlib.h>
#include <string>
#include <map>
#include <list>
#include "data.h"
#include <vector>

using namespace std;

class Exp
{
public:
  virtual Data evaluate(void) = 0;
  virtual void print(void) = 0;
  bool to_bool(void);
};

struct Stmt
{
  virtual void execute(void) = 0;
  virtual void print(void) = 0;
};

struct MultiStmt : Stmt
{
  std::vector<Stmt *> stmts;
  void execute(void);
  void print(void);
  MultiStmt(Stmt *s) : stmts(std::vector<Stmt *>{s}) {}
};

struct Fn
{
  MultiStmt *stmts;
  Fn(MultiStmt *s) : stmts(s) {}
  int fn_call(void);
};

struct IfStmt : Stmt
{
  Exp *cond;
  MultiStmt *t_branch;
  MultiStmt *f_branch;
  IfStmt(Exp *cond, MultiStmt *t, MultiStmt *f) : cond{cond}, t_branch{t}, f_branch{f} {}
  void execute(void);
  void print(void);
};

struct Pass : Stmt
{
  void execute(void);
  void print(void);
};

class AssignStmt : public Stmt
{
protected:
  string id;
  Exp *exp;

public:
  AssignStmt(string name, Exp *expression);
  void print();
  void execute();
};

class PrintStmt : public Stmt
{
protected:
  Exp *exp;

public:
  PrintStmt(Exp *myexp);
  void print();
  void execute();
};

class BinaryExp : public Exp
{
public:
  Exp *left;
  Exp *right;
  BinaryOperator op;
  BinaryExp(Exp *left, BinaryOperator op, Exp *right)
  {
    this->left = left;
    this->op = op;
    this->right = right;
  }
  Data evaluate()
  {
    // Todo check for nullptr ?
    Data a = left->evaluate();
    Data b = right->evaluate();
    return a.apply(&b, this->op);
  };
  void print(void) { cout << "TODO"; };
};

class LiteralExp : public Exp
{
public:
  Data value;
  LiteralExp(Data value) : value{value} {}
  Data evaluate(void)
  {
    return this->value;
  }
  void print(void)
  {
    value.print();
  };
};

class VarExp : public Exp
{
public:
  std::string id;
  VarExp(std::string id) : id({id}) {}
  Data evaluate(void);
  void print(void) { cout << this->id; };
};

// the object at the base of our tree
extern map<string, Data> state;
