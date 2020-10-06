#include <iostream>
#include <stdlib.h>
#include <string>
#include <map>
#include <list>
#include "my-c.h"

using namespace std;

void Pass::execute()
{
}
void Pass::print()
{
  cout << "pass" << endl;
}

int Fn::fn_call(void)
{
  cout << "Calling Fn" << endl;
  for (int i = stmts->stmts.size() - 1; i >= 0; i--)
  {
    auto x = stmts->stmts[i];
    if (x == nullptr)
    {
      cout << "NULL STMT!";
      return -1;
    }
    x->execute();
  }
  return 0;
}

Data VarExp::evaluate()
{
  auto d = state.at(this->id);
  return Data(d);
}

void MultiStmt::execute()
{
  for (int i = 0; i < this->stmts.size(); i++)
  {
    this->stmts[i]->execute();
  }
}
void MultiStmt::print()
{
  for (int i = 0; i < this->stmts.size(); i++)
  {
    this->stmts[i]->print();
  }
}

assign_node::assign_node(std::string name, Exp *expression)
    : id(name), exp(expression) {}

void assign_node::print()
{
  cout << id << " = ";
  exp->print();
  cout << endl;
}

void assign_node::execute()
{
  Data result = exp->evaluate(); // Todo fix unsafety

  state[id] = result;
}

print_node::print_node(Exp *myexp) : exp(myexp) {}

void print_node::print()
{
  cout << "print ";
  exp->print();
  cout << endl;
}

void print_node::execute()
{
  cout << "PRINTING: " << exp->evaluate().i << endl // Todo fix unsafety
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
