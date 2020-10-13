#include "data.h"
#include "stdio.h"
#include "iostream"

void print_op(BinaryOperator op)
{
    switch (op)
    {
    case BinaryOperator::MUL:
        std::cout << "*";
        break;
    case BinaryOperator::DIV:
        std::cout << "/";
        break;
    case BinaryOperator::ADD:
        std::cout << "+";
        break;
    case BinaryOperator::SUB:
        std::cout << "-";
        break;
    case BinaryOperator::BAND:
        std::cout << "&&";
        break;
    case BinaryOperator::BOR:
        std::cout << "||";
        break;
    case BinaryOperator::LTHAN:
        std::cout << "<";
        break;
    case BinaryOperator::LETHAN:
        std::cout << "<=";
        break;
    case BinaryOperator::GTHAN:
        std::cout << ">";
        break;
    case BinaryOperator::GETHAN:
        std::cout << ">=";
        break;
    case BinaryOperator::EQ:
        std::cout << "==";
        break;
    case BinaryOperator::NEQ:
        std::cout << "!=";
        break;
    }
}

Data::Data(bool b) : tag{Data::BOOL}, b{b} {}
Data::Data(int i) : tag{Data::INT}, i{i} {}
Data::Data(float f) : tag{Data::FLOAT}, f{f} {}
Data::Data(char c) : tag{Data::CHAR}, c{c} {}
Data::Data(std::string *s) : tag{Data::STRING}, s{s} {}
Data::Data(Array *a) : tag{Data::ARR}, a{a} {}
Data::Data() : Data(0) {}
Data Data::apply(Data *other, BinaryOperator op)
{
    if (this->tag == other->tag)
    {
        switch (this->tag)
        {
        case Data::INT:
        {
            return num_op(this->i, op, other->i);
        }
        case Data::FLOAT:
        {
            return num_op(this->f, op, other->f);
        }
        case Data::BOOL:
        {
            return bool_op(this->b, op, other->b);
        }
        case Data::STRING:
            return str_op(this->s, op, other->s);
        case Data::ARR:
            std::cout << "Cannot perform binary operations on an array" << std::endl;
        }
    }
    else
    {
        std::cout << "Cannot implicitly convert " << this->tag << " to " << other->tag << std::endl;
    }
    return Data(0);
}

void Data::print()
{
    switch (this->tag)
    {
    case Data::INT:
        std::cout << this->i;
        break;
    case Data::FLOAT:
        std::cout << this->f;
        break;
    case Data::BOOL:
        if (this->b)
        {
            std::cout << "true";
        }
        else
        {
            std::cout << "false";
        }
        break;
    case Data::CHAR:
        std::cout << this->c;
        break;
    case Data::STRING:
        std::cout << *(this->s);
        break;
    case Data::ARR:
    {
        Array *arr = this->a;
        arr->print();
        break;
    }
    }
}

Data bool_op(bool left, BinaryOperator op, bool right)
{
    switch (op)
    {
    case BinaryOperator::BAND:
        return Data(left && right);
    case BinaryOperator::BOR:
        return Data(left || right);
    case BinaryOperator::EQ:
        return Data(left == right);
    case BinaryOperator::NEQ:
        return Data(left != right);
    }
    return false;
}

Data num_op(int left, BinaryOperator op, int right)
{
    switch (op)
    {
    case BinaryOperator::ADD:
        return Data(left + right);
    case BinaryOperator::SUB:
        return Data(left - right);
    case BinaryOperator::MUL:
        return Data(left * right);
    case BinaryOperator::DIV:
        return Data(left / right);
    case BinaryOperator::LTHAN:
        return Data(left < right);
    case BinaryOperator::LETHAN:
        return Data(left <= right);
    case BinaryOperator::GTHAN:
        return Data(left > right);
    case BinaryOperator::GETHAN:
        return Data(left >= right);
    case BinaryOperator::EQ:
        return Data(left == right);
    case BinaryOperator::NEQ:
        return Data(left != right);
    }
    return Data(left);
}

Data num_op(float left, BinaryOperator op, float right)
{
    switch (op)
    {
    case BinaryOperator::ADD:
        return Data(left + right);
    case BinaryOperator::SUB:
        return Data(left - right);
    case BinaryOperator::MUL:
        return Data(left * right);
    case BinaryOperator::DIV:
        return Data(left / right);
    case BinaryOperator::LTHAN:
        return Data(left < right);
    case BinaryOperator::LETHAN:
        return Data(left <= right);
    case BinaryOperator::GTHAN:
        return Data(left > right);
    case BinaryOperator::GETHAN:
        return Data(left >= right);
    case BinaryOperator::EQ:
        return Data(left == right);
    case BinaryOperator::NEQ:
        return Data(left != right);
    }
    return Data(left);
}

Data str_op(std::string *left, BinaryOperator op, std::string *right)
{
    switch (op)
    {
    case BinaryOperator::ADD:
    {
        auto s = new std::string(*left + *right);
        return Data(s);
    }
    case BinaryOperator::EQ:
        return Data(*left == *right);
    case BinaryOperator::NEQ:
        return Data(*left != *right);
    }
    return Data(left);
}
int Array::one_d(std::vector<int> indices)
{
    int mul = 1;
    int index = indices[0];
    for (int i = 1; i < indices.size(); i++)
    {
        mul *= this->sizes[i - 1];
        index += (indices[i] * mul);
    }
    return index;
}
Data Array::get(std::vector<int> indices)
{
    return data[this->one_d(indices)];
}
Data Array::set(std::vector<int> indices, Data val)
{
    data[this->one_d(indices)] = val;
}
Data Array::set(int index, Data val)
{
    data[index] = val;
}
void Array::print()
{
    std::cout << "[";
    for (int i = 0; i < this->total_size; i++)
    {
        this->data[i].print();
        std::cout << ",";
    }
    std::cout << "]";
}

void VarStorage::declare(std::string id, Data init)
{
    this->state[id] = init;
}
void VarStorage::assign(std::string id, Data val)
{
    this->state[id] = val;
}
Data VarStorage::get(std::string *name)
{
    auto d = this->state.at(*name);
    return Data(d);
}