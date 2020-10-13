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
  virtual Data evaluate(VarStorage *state) = 0;
  virtual void print(VarStorage *state) = 0;
  bool to_bool(VarStorage *state);
};

struct Stmt
{
  virtual void execute(VarStorage *state) = 0;
  virtual void print(VarStorage *state) = 0;
  Data ret_val(void)
  {
    return Data(0);
  }
};

struct MultiStmt : Stmt
{
  std::vector<Stmt *> stmts;
  void execute(VarStorage *state);
  void print(VarStorage *state);
  MultiStmt(Stmt *s) : stmts(std::vector<Stmt *>{s}) {}
};

struct Fn
{
  std::string ident;
  MultiStmt *stmts;
  Fn(std::string ident, MultiStmt *s) : stmts(s), ident{ident} {}
  Data fn_call(VarStorage storage);
};

struct IfStmt : Stmt
{
  Exp *cond;
  MultiStmt *t_branch;
  MultiStmt *f_branch;
  IfStmt(Exp *cond, MultiStmt *t, MultiStmt *f) : cond{cond}, t_branch{t}, f_branch{f} {}
  void execute(VarStorage *state);
  void print(VarStorage *state);
};

struct Pass : Stmt
{
  void execute(VarStorage *state);
  void print(VarStorage *state);
};

class AssignStmt : public Stmt
{
protected:
  string id;
  Exp *exp;

public:
  AssignStmt(string name, Exp *expression);
  void print(VarStorage *state);
  void execute(VarStorage *state);
};

class PrintStmt : public Stmt
{
protected:
  Exp *exp;

public:
  PrintStmt(Exp *myexp);
  void print(VarStorage *state);
  void execute(VarStorage *state);
};

class ReturnStmt : public Stmt
{
public:
  Exp *exp;
  Data d;
  ReturnStmt(Exp *exp) : exp{exp}, d(Data(0)) {}
  void print(VarStorage *state);
  void execute(VarStorage *state);
  Data ret_val();
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
  Data evaluate(VarStorage *state)
  {
    // Todo check for nullptr ?
    Data a = left->evaluate(state);
    Data b = right->evaluate(state);
    return a.apply(&b, this->op);
  };
  void print(VarStorage *state) { cout << "TODO BinaryExp PRINT"; };
};

class LiteralExp : public Exp
{
public:
  Data value;
  LiteralExp(Data value) : value{value} {}
  Data evaluate(VarStorage *state)
  {
    return this->value;
  }
  void print(VarStorage *state)
  {
    value.print();
  };
};

class NegationExp : public Exp
{
public:
  Exp *exp;
  NegationExp(Exp *exp) : exp{exp} {}
  Data evaluate(VarStorage *state);
  void print(VarStorage *state);
};

class VarExp : public Exp
{
public:
  std::string id;
  VarExp(std::string id) : id({id}) {}
  Data evaluate(VarStorage *state);
  void print(VarStorage *state) { cout << this->id; };
};

// the object at the base of our tree
// extern map<string, Data> state;
