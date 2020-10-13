#include <iostream>
#include <stdlib.h>
#include <string>
#include <map>
#include <list>
#include "my-c.h"

using namespace std;

bool Exp::to_bool(VarStorage *state)
{
  Data d = this->evaluate(state);
  if (d.tag == Data::BOOL)
  {
    return d.b;
  }
  std::cout << "Expected boolean got ";
  d.print();
  std::cout << endl;
  std::cout << "Implicit conversion of boolean to false";
  return false;
}

void MultiStmt::execute(VarStorage *state)
{
  for (int i = 0; i < this->stmts.size(); i++)
  {
    this->stmts[i]->execute(state);
  }
}
void MultiStmt::print(VarStorage *state)
{
  for (int i = 0; i < this->stmts.size(); i++)
  {
    this->stmts[i]->print(state);
  }
}

Data Fn::fn_call(VarStorage storage)
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
    x->execute(&storage);
  }
  return stmts->stmts[0]->ret_val();
}

void IfStmt::execute(VarStorage *state)
{
  if (this->cond->to_bool(state))
  {
    this->t_branch->execute(state);
  }
  else if (this->f_branch != nullptr)
  {
    this->f_branch->execute(state);
  }
}
void IfStmt::print(VarStorage *state)
{
  cout << "TODO IF STMT PRINT" << endl;
}

void Pass::execute(VarStorage *state)
{
}
void Pass::print(VarStorage *state)
{
  cout << "pass" << endl;
}

AssignStmt::AssignStmt(std::string name, Exp *expression)
    : id(name), exp(expression) {}

void AssignStmt::print(VarStorage *state)
{
  cout << id << " = ";
  exp->print(state);
  cout << endl;
}

void AssignStmt::execute(VarStorage *state)
{
  Data result = exp->evaluate(state);
  state->assign(id, result);
}

PrintStmt::PrintStmt(Exp *myexp) : exp(myexp) {}

void PrintStmt::print(VarStorage *state)
{
  cout << "print ";
  exp->print(state);
  cout << endl;
}

void PrintStmt::execute(VarStorage *state)
{
  cout << "PRINTING: ";
  exp->evaluate(state).print();
  cout << endl;
}

void ReturnStmt::execute(VarStorage *state)
{
  this->d = exp->evaluate(state);
}

void ReturnStmt::print(VarStorage *state)
{
  cout << "Return";
  exp->print(state);
  cout << endl;
}

Data ReturnStmt::ret_val()
{
  return this->d;
}

Data NegationExp::evaluate(VarStorage *state)
{
  Data d = this->exp->evaluate(state);
  if (d.tag == Data::BOOL)
  {
    bool b = d.b;
    return Data(!b);
  }
  std::cout << "Expected boolean got ";
  d.print();
  std::cout << endl;
  std::cout << "Implicit conversion of boolean to false";

  return Data(true);
}
void NegationExp::print(VarStorage *state)
{
  cout << "!";
  this->exp->print(state);
}

Data VarExp::evaluate(VarStorage *state)
{
  return state->get(&this->id);
}

// map<string, Data> state;
