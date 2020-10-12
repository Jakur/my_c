#include <iostream>
#include <stdlib.h>
#include <string>
#include <map>
#include <list>
#include "my-c.h"

using namespace std;

bool Exp::to_bool()
{
  Data d = this->evaluate();
  if (d.tag == Data::BOOL)
  {
    return d.b;
  }
  std::cout << "Implicit conversion of boolean to false";
  return false;
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

void IfStmt::execute(void)
{
  if (this->cond->to_bool())
  {
    this->t_branch->execute();
  }
  else if (this->f_branch != nullptr)
  {
    this->f_branch->execute();
  }
}
void IfStmt::print(void)
{
  cout << "TODO IF STMT PRINT" << endl;
}

void Pass::execute()
{
}
void Pass::print()
{
  cout << "pass" << endl;
}

AssignStmt::AssignStmt(std::string name, Exp *expression)
    : id(name), exp(expression) {}

void AssignStmt::print()
{
  cout << id << " = ";
  exp->print();
  cout << endl;
}

void AssignStmt::execute()
{
  Data result = exp->evaluate(); // Todo fix unsafety

  state[id] = result;
}

PrintStmt::PrintStmt(Exp *myexp) : exp(myexp) {}

void PrintStmt::print()
{
  cout << "print ";
  exp->print();
  cout << endl;
}

void PrintStmt::execute()
{
  cout << "PRINTING: " << exp->evaluate().i << endl // Todo fix unsafety
       << endl;
}

Data VarExp::evaluate()
{
  auto d = state.at(this->id);
  return Data(d);
}

//I'm not sure if I was suposed to do all of these or if there is anything else to add
//would this work for arrays
Data ArrayExp::evaluate()
{
  auto a = state.at(this->id);
  return Data(a);
}

//Is this for loop correct or should I be printing it differently?
void ArrayExp::printArray()
{
  cout << "Array " << id << " values: ";
  for (int i = 0; i < this->Exp.size() -1; i++)
  {
    cout << this->Exp[i] << ", ";
  }
  cout << endl;
}

//Printing specific index, need feedback if this is correct
void ArrayExp::printIndex()
{
  cout << "Array " << id << " value: ";
  cout << this->Exp[this->id] << endl;
}

//Would this add a value at the index?
void ArrayExp::execute()
{
  Data result = exp->evaluate();
  Exp[id] = result;
}

map<string, Data> state;
