#include <string>

enum class BinaryOperator
{
    MUL,
    DIV,
    ADD,
    SUB,
    AND,
    OR,
    LTHAN,
    LETHAN,
    GTHAN,
    GETHAN,
    EQ,
    NEQ
};

class Data
{
public:
    enum
    {
        INT,
        CHAR,
        FLOAT,
    } tag;
    union {
        int i;
        char c;
        float f;
    };
    Data(int i);
    Data(char c);
    Data(float f);
    Data();
    Data apply(Data *other, BinaryOperator op);
    void print();
};

template <class T>
T num_op(T left, BinaryOperator op, T right)
{
    switch (op)
    {
    case BinaryOperator::ADD:
        return left + right;
    case BinaryOperator::SUB:
        return left - right;
    case BinaryOperator::MUL:
        return left * right;
    case BinaryOperator::DIV:
        return left / right;
    default:
        return left; // Todo finish operations
    }
}

// std::ostream &operator<<(std::ostream &os, Data const &m);