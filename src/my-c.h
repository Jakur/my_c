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
class FlowGraph;

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
  virtual void compute_flow(FlowGraph *g, int prev, int next);
  virtual int label() = 0;
  virtual std::optional<std::string> gen_set();
  virtual std::optional<std::string> kill_set();
};

struct MultiStmt : Stmt
{
  std::vector<Stmt *> stmts;
  std::optional<Data> execute(VarStorage *state);
  void print(VarStorage *state);
  MultiStmt(Stmt *s) : stmts(std::vector<Stmt *>{s}) {}
  void compute_flow(FlowGraph *g, int prev, int next) override;
  int label() { return this->stmts[stmts.size() - 1]->label(); };
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
  int lab;
  Exp *cond;
  MultiStmt *t_branch;
  MultiStmt *f_branch;
  IfStmt(Exp *cond, MultiStmt *t, MultiStmt *f) : cond{cond}, t_branch{t}, f_branch{f}, lab{label_num++} {}
  std::optional<Data> execute(VarStorage *state);
  void print(VarStorage *state);
  int label() { return lab; };
  void compute_flow(FlowGraph *g, int prev, int next) override;
};

struct WhileStmt : Stmt
{
  int lab;
  Exp *cond;
  MultiStmt *body;
  WhileStmt(Exp *cond, MultiStmt *body) : cond{cond}, body{body}, lab{label_num++} {}
  std::optional<Data> execute(VarStorage *state);
  void print(VarStorage *state);
  void compute_flow(FlowGraph *g, int prev, int next) override;
  int label() { return lab; };
};

struct Pass : Stmt
{
  int lab;
  std::optional<Data> execute(VarStorage *state);
  void print(VarStorage *state);
  Pass() : lab{label_num++} {}
  int label() { return lab; };
};

class AssignStmt : public Stmt
{
protected:
  string id;
  Exp *exp;

public:
  int lab;
  void print(VarStorage *state);
  std::optional<Data> execute(VarStorage *state);
  AssignStmt(std::string name, Exp *expression)
      : id(name), exp(expression), lab{label_num++} {}
  int label() { return lab; };
  std::optional<std::string> gen_set() override;
  std::optional<std::string> kill_set() override;
};

class PrintStmt : public Stmt
{
protected:
  Exp *exp;

public:
  int lab;
  void print(VarStorage *state);
  std::optional<Data> execute(VarStorage *state);
  PrintStmt(Exp *myexp) : exp(myexp), lab{label_num++} {}
  int label() { return lab; };
};

class ReturnStmt : public Stmt
{
public:
  int lab;
  Exp *exp;
  Data d;
  ReturnStmt(Exp *exp) : exp{exp}, d(Data(0)), lab{label_num++} {}
  void print(VarStorage *state);
  std::optional<Data> execute(VarStorage *state);
  int label() { return lab; };
};

class DeclareStmt : public Stmt
{
public:
  int lab;
  string id;
  DeclareStmt(string id) : id{id}, lab{label_num++} {}
  void print(VarStorage *state);
  std::optional<Data> execute(VarStorage *state);
  int label() { return lab; };
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
  int lab;
  std::vector<Exp *> exps;
  std::vector<Data> evaluated;
  void print(VarStorage *state);
  std::optional<Data> execute(VarStorage *state);
  ExpList(Exp *exp) : exps(std::vector<Exp *>{exp}), evaluated(std::vector<Data>()), lab{label_num++} {}
  int label() { return lab; };
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
  int lab;
  std::vector<std::string> names;
  void print(VarStorage *state);
  std::optional<Data> execute(VarStorage *state);
  ParmList(std::string s) : names(std::vector<std::string>{s}), lab{label_num++} {}
  int label() { return lab; };
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

class FlowGraph
{
public:
  FlowGraph() : edges{}, rev_edges{}, nodes{} {}
  std::map<int, Stmt *> nodes;
  std::map<int, std::set<int>> edges;
  std::map<int, std::set<int>> rev_edges;
  void add_node(int label_id, Stmt *stmt);
  void add_edge(int src, int dest);
  void print_edges();
  void print_rev_edges();
};

void print_label(int label);

// the object at the base of our tree
extern map<string, Fn *> fns;
