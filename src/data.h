#include <optional>
#include <string>
#include <variant>

enum BinaryOperator
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
    Data apply(Data *other, BinaryOperator op);
};

template <class T>
T num_op(T left, BinaryOperator op, T right)
{
    switch (op)
    {
    case ADD:
        return left + right;
    case SUB:
        return left - right;
    case MUL:
        return left * right;
    case DIV:
        return left / right;
    default:
        return left; // Todo finish operations
    }
}