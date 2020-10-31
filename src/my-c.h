#include <iostream>
#include <stdlib.h>
#include <string>
#include <map>
#include <list>
#include <optional>
#include "data.h"
#include <vector>

using namespace std;

static int label_num = 0;

struct DataType;

class Exp
{
public:
  virtual Data evaluate(VarStorage *state) = 0;
  virtual void print(VarStorage *state) = 0;
  bool to_bool(VarStorage *state);
};

struct Stmt
{
  virtual std::optional<Data> execute(VarStorage *state) = 0;
  virtual void print(VarStorage *state) = 0;
};

struct MultiStmt : Stmt
{
  int label;
  std::vector<Stmt *> stmts;
  std::optional<Data> execute(VarStorage *state);
  void print(VarStorage *state);
  MultiStmt(Stmt *s) : stmts(std::vector<Stmt *>{s}), label{label_num++} {}
};

struct Fn
{
  std::string ident;
  std::vector<std::string> parameters;
  MultiStmt *stmts;
  Fn(std::string ident, std::vector<std::string> parms, MultiStmt *s) : stmts(s), ident{ident}, parameters{parms} {}
  Data fn_call(VarStorage storage);
  void print();
};

struct IfStmt : Stmt
{
  int label;
  Exp *cond;
  MultiStmt *t_branch;
  MultiStmt *f_branch;
  IfStmt(Exp *cond, MultiStmt *t, MultiStmt *f) : cond{cond}, t_branch{t}, f_branch{f}, label{label_num++} {}
  std::optional<Data> execute(VarStorage *state);
  void print(VarStorage *state);
};

struct WhileStmt : Stmt
{
  int label;
  Exp *cond;
  MultiStmt *body;
  WhileStmt(Exp *cond, MultiStmt *body) : cond{cond}, body{body}, label{label_num++} {}
  std::optional<Data> execute(VarStorage *state);
  void print(VarStorage *state);
};

struct Pass : Stmt
{
  int label;
  std::optional<Data> execute(VarStorage *state);
  void print(VarStorage *state);
  Pass() : label{label_num++} {}
};

class AssignStmt : public Stmt
{
protected:
  string id;
  Exp *exp;

public:
  int label;
  void print(VarStorage *state);
  std::optional<Data> execute(VarStorage *state);
  AssignStmt(std::string name, Exp *expression)
      : id(name), exp(expression), label{label_num++} {}
};

class PrintStmt : public Stmt
{
protected:
  Exp *exp;

public:
  int label;
  void print(VarStorage *state);
  std::optional<Data> execute(VarStorage *state);
  PrintStmt(Exp *myexp) : exp(myexp), label{label_num++} {}
};

class ReturnStmt : public Stmt
{
public:
  int label;
  Exp *exp;
  Data d;
  ReturnStmt(Exp *exp) : exp{exp}, d(Data(0)), label{label_num++} {}
  void print(VarStorage *state);
  std::optional<Data> execute(VarStorage *state);
};

class DeclareStmt : public Stmt
{
public:
  int label;
  string id;
  DeclareStmt(string id) : id{id}, label{label_num++} {}
  void print(VarStorage *state);
  std::optional<Data> execute(VarStorage *state);
};

class BinaryExp : public Exp
{
public:
  Exp *left;
  Exp *right;
  BinaryOperator op;
  BinaryExp(Exp *left, BinaryOperator op, Exp *right) : left{left}, op{op}, right{right} {}
  Data evaluate(VarStorage *state);
  void print(VarStorage *state);
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
  int label;
  std::vector<Exp *> exps;
  std::vector<Data> evaluated;
  void print(VarStorage *state);
  std::optional<Data> execute(VarStorage *state);
  ExpList(Exp *exp) : exps(std::vector<Exp *>{exp}), evaluated(std::vector<Data>()), label{label_num++} {}
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
  int label;
  std::vector<std::string> names;
  void print(VarStorage *state);
  std::optional<Data> execute(VarStorage *state);
  ParmList(std::string s) : names(std::vector<std::string>{s}), label{label_num++} {}
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

struct DataType : public Exp
{
  Data init;
  ExpList *e;
  Data evaluate(VarStorage *state);
  void print(VarStorage *state);
  DataType(Data init) : init{init} {}
  DataType(Data init, ExpList *e) : init{init}, e{e} {}
};

class ArrayInitExp : public Exp
{
public:
  DataType *ty;
  ExpList *values;
  ArrayInitExp(DataType *ty, ExpList *values) : ty{ty}, values{values} {}
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
