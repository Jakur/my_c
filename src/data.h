#include <string>
#include <map>

enum class BinaryOperator
{
    MUL,
    DIV,
    ADD,
    SUB,
    BAND,
    BOR,
    LTHAN,
    LETHAN,
    GTHAN,
    GETHAN,
    EQ,
    NEQ
};

class VarStorage
{
};

class Data
{
public:
    enum
    {
        INT,
        CHAR,
        BOOL,
        FLOAT,
    } tag;
    union {
        int i;
        char c;
        float f;
        bool b;
    };
    Data(int i);
    Data(char c);
    Data(float f);
    Data(bool b);
    Data();
    Data apply(Data *other, BinaryOperator op);
    void print();
};

Data bool_op(bool left, BinaryOperator op, bool right);

template <class T>
Data num_op(T left, BinaryOperator op, T right)
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

// std::ostream &operator<<(std::ostream &os, Data const &m);