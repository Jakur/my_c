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

class exp_node
{
public:
  // print function for pretty printing an expression
  virtual void print() = 0;

  // evaluation function for a leaf, replaced for interior nodes
  virtual float evaluate() = 0;
};

class binaryOp_node : public exp_node
{
public:
  exp_node *left;
  exp_node *right;

  // the constructor for node links the node to its children,
  // and stores the character representation of the operator.
  binaryOp_node(exp_node *L, exp_node *R);
};

class number_node : public exp_node
{
private:
  float num;

public:
  number_node(float value);
  void print();
  float evaluate();
};

class neg_node : public exp_node
{
protected:
  exp_node *exp;

public:
  neg_node(exp_node *exp);
  void print();
  float evaluate();
};

class variable_node : public exp_node
{
protected:
  string id;

public:
  variable_node(string value);
  void print();
  float evaluate();
};

// add_node inherits the characteristics of node and adds its own evaluate function
class add_node : public binaryOp_node
{
public:
  // add_node's constructor just uses node's constructor
  add_node(exp_node *L, exp_node *R);
  void print();
  float evaluate();
};

// subtract_node inherits the characteristics of node and adds its own evaluate function
class subtract_node : public binaryOp_node
{
public:
  subtract_node(exp_node *L, exp_node *R);
  void print();
  float evaluate();
};

// multiply_node inherits the characteristics of node and adds its own evaluate function
class multiply_node : public binaryOp_node
{
public:
  multiply_node(exp_node *L, exp_node *R);
  void print();
  float evaluate();
};

// divide_node inherits the characteristics of node and adds its own evaluate function
class divide_node : public binaryOp_node
{
public:
  divide_node(exp_node *L, exp_node *R);
  void print();
  float evaluate();
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
  exp_node *exp;

public:
  assign_node(string name, exp_node *expression);
  void print();
  void evaluate();
};

class print_node : public stmt_node
{
protected:
  exp_node *exp;

public:
  print_node(exp_node *myexp);
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
extern map<string, float> state;
