#include <iostream>
#include <stdlib.h>
#include <string>
#include <map>
#include <list>
#include "my-c.h"
#include <algorithm>

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

void Fn::print()
{
  VarStorage st = VarStorage(map<string, Data>());
  this->stmts->print(&st);
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
  cout << "if ";
  this->cond->print(state);
  cout << " then ";
  this->t_branch->print(state);
  if (this->f_branch != nullptr)
  {
    cout << " else ";
    this->f_branch->print(state);
  }
  cout << "end" << endl;
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
  cout << "while ";
  this->cond->print(state);
  cout << "do ";
  this->body->print(state);
  cout << "end " << endl;
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
  if (this->evaluated.size() > 0)
  {
    std::cout << "Warning evaluating already used ExpList!" << std::endl;
  }
  for (int i = this->exps.size() - 1; i >= 0; i--)
  {
    Data d = this->exps[i]->evaluate(state);
    this->evaluated.push_back(d);
  }
  return {};
}

Data BinaryExp::evaluate(VarStorage *state)
{
  // Todo check for nullptr ?
  Data a = left->evaluate(state);
  Data b = right->evaluate(state);
  return a.apply(&b, this->op);
};

void BinaryExp::print(VarStorage *state)
{
  std::cout << "(";
  this->left->print(state);
  print_op(this->op);
  this->right->print(state);
  std::cout << ")";
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
  if (this->ty == nullptr)
  {
    std::cout << "Warning DataType is null!";
  }
  auto arr_data = this->ty->evaluate(state);
  auto arr = arr_data.a;
  if (size != arr->total_size)
  {
    std::cout << "Warning array not fully initialized" << std::endl;
  }
  std::reverse(this->values->exps.begin(), this->values->exps.end());
  for (int i = 0; i < size; i++)
  {
    arr->set(i, this->values->exps[i]->evaluate(state));
  }
  std::cout << "Printing off: " << std::endl;
  for (int j = 0; j < 3; j++)
  {
    for (int i = 0; i < 2; i++)
    {
      Data d = arr->get(std::vector<int>{i, j});
      if (d.tag == Data::INT)
      {
        std::cout << arr->get(std::vector<int>{i, j}).i;
      }
    }
  }
  std::cout << std::endl;
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

Data DataType::evaluate(VarStorage *state)
{
  if (this->e != nullptr)
  {
    this->e->execute(state);
    int total = 1;
    auto sizes = std::vector<int>();
    for (int i = 0; i < this->e->evaluated.size(); ++i)
    {
      auto d = this->e->evaluated[i];
      if (d.tag == Data::INT)
      {
        sizes.push_back(d.i);
        total *= d.i;
      }
      else
      {
        std::cout << "Array index size is not an integer! Defaulting to size 1" << std::endl;
        sizes.push_back(1);
      }
    }
    auto arr_data = std::vector<Data>(total, this->init);
    std::reverse(sizes.begin(), sizes.end());
    return Data(new Array(total, sizes, arr_data));
  }
  else
  {
    return this->init;
  }
}

void DataType::print(VarStorage *state)
{
  if (this->e != nullptr)
  {
    std::cout << "Array with initial value";
    this->init.print();
  }
  else
  {
    std::cout << "Type with initial value";
    this->init.print();
  }
}

map<string, Fn *> fns;
