#include <iostream>
#include <stdlib.h>
#include <string>
#include <map>
#include <list>
#include <optional>
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
  virtual std::optional<Data> ret_val() = 0;
  // std::optional<Data> ret_val(void)
  // {
  //   cout << "Basic bitches" << endl;
  //   return std::optional<Data>();
  // }
};

struct MultiStmt : Stmt
{
  std::vector<Stmt *> stmts;
  void execute(VarStorage *state);
  void print(VarStorage *state);
  std::optional<Data> ret_val() { return std::optional<Data>(); }
  MultiStmt(Stmt *s) : stmts(std::vector<Stmt *>{s}) {}
};

struct Fn
{
  std::string ident;
  std::vector<std::string> parameters;
  MultiStmt *stmts;
  Fn(std::string ident, std::vector<std::string> parms, MultiStmt *s) : stmts(s), ident{ident}, parameters{parms} {}
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
  std::optional<Data> ret_val() { return std::optional<Data>(); }
};

struct WhileStmt : Stmt
{
  Exp *cond;
  MultiStmt *body;
  WhileStmt(Exp *cond, MultiStmt *body) : cond{cond}, body{body} {}
  void execute(VarStorage *state);
  void print(VarStorage *state);
  std::optional<Data> ret_val() { return std::optional<Data>(); }
};

struct Pass : Stmt
{
  void execute(VarStorage *state);
  void print(VarStorage *state);
  std::optional<Data> ret_val() { return std::optional<Data>(); }
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
  std::optional<Data> ret_val() { return std::optional<Data>(); }
};

class PrintStmt : public Stmt
{
protected:
  Exp *exp;

public:
  PrintStmt(Exp *myexp);
  void print(VarStorage *state);
  void execute(VarStorage *state);
  std::optional<Data> ret_val() { return std::optional<Data>(); }
};

class ReturnStmt : public Stmt
{
public:
  Exp *exp;
  Data d;
  ReturnStmt(Exp *exp) : exp{exp}, d(Data(0)) {}
  void print(VarStorage *state);
  void execute(VarStorage *state);
  std::optional<Data> ret_val();
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

class ExpList : public Stmt
{
public:
  std::vector<Exp *> exps;
  std::vector<Data> evaluated;
  void print(VarStorage *state);
  void execute(VarStorage *state);
  ExpList(Exp *exp) : exps(std::vector<Exp *>{exp}), evaluated(std::vector<Data>()) {}
  std::optional<Data> ret_val() { return std::optional<Data>(); }
};

class FnCallExp : public Exp
{
public:
  std::string name;
  ExpList *exps;
  FnCallExp(std::string name, ExpList *exps) : name{name}, exps{exps} {}
  Data evaluate(VarStorage *state);
  void print(VarStorage *state);
};

class ParmList : public Stmt
{
public:
  std::vector<std::string> names;
  void print(VarStorage *state);
  void execute(VarStorage *state);
  ParmList(std::string s) : names(std::vector<std::string>{s}) {}
  std::optional<Data> ret_val() { return std::optional<Data>(); }
};

class IndexExp : public Exp
{
public:
  std::string id;
  ExpList *indices;
  IndexExp(std::string id, ExpList *indices) : id{id}, indices{indices} {}
  Data evaluate(VarStorage *state);
  void print(VarStorage *state);
};

class ArrayInitExp : public Exp
{
public:
  ExpList *values;
  ArrayInitExp(ExpList *values) : values{values} {}
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
extern map<string, Fn *> fns;
