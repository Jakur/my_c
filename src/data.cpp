#include "data.h"

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
            int res = num_op(this->i, op, other->i);
            return Data(res);
        }
        case Data::FLOAT:
        {
            float res = num_op(this->f, op, other->f);
            return Data(res);
        }
        }
    }

    else
    {
        // Todo implicit casting?
    }
    return Data(0);
}