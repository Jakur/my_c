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

std::optional<Data> MultiStmt::execute(VarStorage *state)
{
  for (int i = this->stmts.size() - 1; i >= 0; i--)
  {
    auto res = this->stmts[i]->execute(state);
    if (res.has_value())
    {
      return res;
    }
  }
  return {};
}
void MultiStmt::print(VarStorage *state)
{
  for (int i = this->stmts.size() - 1; i >= 0; i--)
  {
    this->stmts[i]->print(state);
  }
}

Data Fn::fn_call(VarStorage storage)
{
  auto ret = this->stmts->execute(&storage);
  if (ret.has_value())
  {
    return ret.value();
  }

  return Data(0);
}

std::optional<Data> IfStmt::execute(VarStorage *state)
{
  if (this->cond->to_bool(state))
  {
    return this->t_branch->execute(state);
  }
  else if (this->f_branch != nullptr)
  {
    return this->f_branch->execute(state);
  }
  return {};
}
void IfStmt::print(VarStorage *state)
{
  cout << "TODO IF STMT PRINT" << endl;
}

std::optional<Data> WhileStmt::execute(VarStorage *state)
{
  while (this->cond->to_bool(state))
  {
    auto res = this->body->execute(state);
    if (res.has_value())
    {
      return res;
    }
  }
  return {};
}
void WhileStmt::print(VarStorage *state)
{
  cout << "TODO WHILE STMT PRINT" << endl;
}

std::optional<Data> Pass::execute(VarStorage *state)
{
  return {};
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

std::optional<Data> AssignStmt::execute(VarStorage *state)
{
  Data result = exp->evaluate(state);
  state->assign(id, result);
  return {};
}

PrintStmt::PrintStmt(Exp *myexp) : exp(myexp) {}

void PrintStmt::print(VarStorage *state)
{
  cout << "print ";
  exp->print(state);
  cout << endl;
}

std::optional<Data> PrintStmt::execute(VarStorage *state)
{
  cout << "PRINTING: ";
  exp->evaluate(state).print();
  cout << endl;
  return {};
}

std::optional<Data> ReturnStmt::execute(VarStorage *state)
{
  return exp->evaluate(state);
}

void ReturnStmt::print(VarStorage *state)
{
  cout << "Return ";
  exp->print(state);
  cout << endl;
}

Data FnCallExp::evaluate(VarStorage *state)
{
  Fn *f = fns[this->name];
  auto next_map = std::map<std::string, Data>();
  if (this->exps != nullptr)
  {
    this->exps->execute(state);
    int start = f->parameters.size() - 1;
    for (int i = start; i >= 0; i--)
    {
      // One is reversed relative to the other
      next_map[f->parameters[i]] = this->exps->evaluated[start - i];
    }
  }
  Data d = f->fn_call(VarStorage(next_map));
  return d;
}
void FnCallExp::print(VarStorage *state)
{
  cout << "TODO FnCallExp print" << endl;
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

void ExpList::print(VarStorage *state)
{
  for (int i = this->exps.size() - 1; i >= 0; i--)
  {
    this->exps[i]->print(state);
    std::cout << ", ";
  }
  std::cout << endl;
}
std::optional<Data> ExpList::execute(VarStorage *state)
{
  for (int i = this->exps.size() - 1; i >= 0; i--)
  {
    Data d = this->exps[i]->evaluate(state);
    this->evaluated.push_back(d);
  }
  return {};
}

void ParmList::print(VarStorage *state)
{
  std::cout << "TODO Parameter List Print" << std::endl;
}
std::optional<Data> ParmList::execute(VarStorage *state)
{
  return {};
}

Data IndexExp::evaluate(VarStorage *state)
{
  Data d = state->get(&this->id);
  if (d.tag == Data::ARR)
  {
    std::vector<int> x{};
    for (int i = 0; i < this->indices->exps.size(); i++)
    {
      Data d = this->indices->exps[i]->evaluate(state);
      if (d.tag == Data::INT)
      {
        x.push_back(d.i);
      }
      else
      {
        std::cout << "Index evaluated to non-int!" << std::endl;
        x.push_back(0);
      }
    }
    return d.a->get(x);
  }
  else
  {
    std::cout << "Tried to index into a non-array!" << std::endl;
  }
  return Data(0);
}
void IndexExp::print(VarStorage *state)
{
  std::cout << "TODO INDEX EXP PRINT" << std::endl;
}

Data ArrayInitExp::evaluate(VarStorage *state)
{
  int size = this->values->exps.size();
  auto arr = new Array(size, std::vector<int>{size});
  for (int i = 0; i < size; i++)
  {
    arr->set(i, this->values->exps[size - 1 - i]->evaluate(state));
  }
  return Data(arr);
}
void ArrayInitExp::print(VarStorage *state)
{
  std::cout << "TODO ARRAY INIT EXP PRINT" << std::endl;
}

Data VarExp::evaluate(VarStorage *state)
{
  return state->get(&this->id);
}

map<string, Fn *> fns;
