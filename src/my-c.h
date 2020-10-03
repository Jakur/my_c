#include <iostream>
#include <stdlib.h>
#include <string>
#include <map>
#include <list>
#include "data.h"

using namespace std;

class Exp
{
public:
  virtual Data evaluate(void) = 0;
  virtual void print(void) = 0;
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
  void print(void){};
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
  void print(void){};
};

class VarExp : public Exp
{
public:
  std::string id;
  VarExp(std::string id)
  {
    this->id = id;
  }
  Data evaluate(void);
  void print(void){};
};

class stmt_node
{
public:
  virtual void print() {}
  virtual void evaluate() = 0;
};

class assign_node : public stmt_node
{
protected:
  string id;
  Exp *exp;

public:
  assign_node(string name, Exp *expression);
  void print();
  void evaluate();
};

class print_node : public stmt_node
{
protected:
  Exp *exp;

public:
  print_node(Exp *myexp);
  void print();
  void evaluate();
};

class skip_node : public stmt_node
{
public:
  skip_node();
  void print();
  void evaluate();
};

class sequence_node : public stmt_node
{
protected:
  stmt_node *stmt1, *stmt2;

public:
  sequence_node(stmt_node *mystmt1, stmt_node *mystmt2);
  void print();
  void evaluate();
};

// the object at the base of our tree
extern map<string, Data> state;
