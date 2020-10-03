#include <iostream>
#include <stdlib.h>
#include <string>
#include <map>
#include <list>
#include "my-c.h"

using namespace std;

Data VarExp::evaluate()
{
  auto d = state[this->id];
  // float f = d.f; // Todo fix the state map
  return Data(d);
}

assign_node::assign_node(std::string name, Exp *expression)
    : id(name), exp(expression) {}

void assign_node::print()
{
  cout << id << " = ";
  exp->print();
}

void assign_node::evaluate()
{
  Data result = exp->evaluate(); // Todo fix unsafety

  state[id] = result;
}

print_node::print_node(Exp *myexp) : exp(myexp) {}

void print_node::print()
{
  cout << "print ";
  exp->print();
}

void print_node::evaluate()
{
  cout << "output: " << exp->evaluate().f << endl // Todo fix unsafety
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

map<string, Data> state;
