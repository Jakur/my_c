#include "data.h"
#include "stdio.h"
#include "iostream"

Data::Data(bool b) : tag{Data::BOOL}, b{b} {}
Data::Data(int i) : tag{Data::INT}, i{i} {}
Data::Data(float f) : tag{Data::FLOAT}, f{f} {}
Data::Data(std::string *s) : tag{Data::STRING}, s{s} {}
Data::Data() : Data(0) {}
Data Data::to_bool(bool b)
{
    return Data(b);
}
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
        }
    }

    else
    {
        std::cout << "Cannot implicitly convert " << this->tag << " to " << other->tag << std::endl;
        // Todo implicit casting?
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
    case Data::CHAR:
        std::cout << this->c;
        break;
    case Data::STRING:
        std::cout << *(this->s);
        break;
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

// std::ostream &operator<<(std::ostream &os, Data const &m)
// {
//     switch (m.tag)
//     {
//     case Data::INT:
//         return os << 1;
//     case Data::FLOAT:
//         return os << 2;
//     }
//     return os;
// }