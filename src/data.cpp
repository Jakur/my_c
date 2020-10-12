#include "data.h"
#include "stdio.h"
#include "iostream"

Data::Data(bool b) : tag{Data::BOOL}, b{b} {}
Data::Data(int i) : tag{Data::INT}, i{i} {}
Data::Data(float f) : tag{Data::FLOAT}, f{f} {}
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
    }
    return false;
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