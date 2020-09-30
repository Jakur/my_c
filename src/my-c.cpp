#include <iostream>
#include <stdlib.h>
#include <string>
#include <map>
#include <list>
#include "my-c.h"

using namespace std;

Data VarExp::evaluate()
{
  float f = state[this->id]; // Todo fix the state map
  return Data(f);
}

// the constructor for node links the node to its children,
// and stores the character representation of the operator.
binaryOp_node::binaryOp_node(exp_node *L, exp_node *R)
{
  left = L;
  right = R;
}

number_node::number_node(float value)
{
  num = value;
}

void number_node::print()
{
  cout << num;
}

float number_node::evaluate()
{
  return num;
}

variable_node::variable_node(string value) : id(value) {}

void variable_node::print()
{
  cout << id;
}

float variable_node::evaluate()
{
  return state[id];
}

// add_node inherits the characteristics of node and adds its own evaluate function
// add_node's constructor just uses node's constructor
add_node::add_node(exp_node *L, exp_node *R) : binaryOp_node(L, R)
{
}

void add_node::print()
{
  cout << "(";
  left->print();
  cout << " + ";
  right->print();
  cout << ")";
}

float add_node::evaluate()
{
  float left_num, right_num;

  left_num = left->evaluate();
  right_num = right->evaluate();

  return left_num + right_num;
}

// subtract_node inherits the characteristics of node and adds its own evaluate function
subtract_node::subtract_node(exp_node *L, exp_node *R) : binaryOp_node(L, R)
{
}

void subtract_node::print()
{
  cout << "(";
  left->print();
  cout << " - ";
  right->print();
  cout << ")";
}

float subtract_node::evaluate()
{
  float left_num, right_num;

  left_num = left->evaluate();
  right_num = right->evaluate();

  return left_num - right_num;
}

// multiply_node inherits the characteristics of node and adds its own evaluate function
multiply_node::multiply_node(exp_node *L, exp_node *R) : binaryOp_node(L, R)
{
}

void multiply_node::print()
{
  cout << "(";
  left->print();
  cout << " * ";
  right->print();
  cout << ")";
}

float multiply_node::evaluate()
{
  float left_num, right_num;

  left_num = left->evaluate();
  right_num = right->evaluate();

  return left_num * right_num;
}

// divide_node inherits the characteristics of node and adds its own evaluate function

divide_node::divide_node(exp_node *L, exp_node *R) : binaryOp_node(L, R)
{
}

void divide_node::print()
{
  cout << "(";
  left->print();
  cout << " / ";
  right->print();
  cout << ")";
}

float divide_node::evaluate()
{
  float left_num, right_num;

  left_num = left->evaluate();
  right_num = right->evaluate();

  if (right_num)
  {
    return left_num / right_num;
  }
  else
  {
    cout << "division by zero -> " << left_num << " / " << 0 << endl;
    //  include stdlib.h for exit
    exit(1);
  }
}

neg_node::neg_node(exp_node *L) : exp(L) {}

void neg_node::print()
{
  cout << "- ( ";
  exp->print();
  cout << " )";
}

float neg_node::evaluate()
{
  float expValue = exp->evaluate();
  return -expValue;
}

assign_node::assign_node(std::string name, exp_node *expression)
    : id(name), exp(expression) {}

void assign_node::print()
{
  cout << id << " = ";
  exp->print();
}

void assign_node::evaluate()
{
  float result = exp->evaluate();

  state[id] = result;
}

print_node::print_node(exp_node *myexp) : exp(myexp) {}

void print_node::print()
{
  cout << "print ";
  exp->print();
}

void print_node::evaluate()
{
  cout << "output: " << exp->evaluate() << endl
       << endl;
}

skip_node::skip_node() {}

void skip_node::evaluate() {}
void skip_node::print() {}

sequence_node::sequence_node(stmt_node *mystmt1, stmt_node *mystmt2) : stmt1(mystmt1), stmt2(mystmt2)
{
}

void sequence_node::print()
{
  stmt1->print();
  cout << " ;" << endl;
  stmt2->print();
}

void sequence_node::evaluate()
{
  stmt1->evaluate();
  stmt2->evaluate();
}

map<string, float> state;
