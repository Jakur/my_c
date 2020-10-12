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
    Data to_bool(bool b);
    Data apply(Data *other, BinaryOperator op);
    void print();
};

Data bool_op(bool left, BinaryOperator op, bool right);
Data num_op(int left, BinaryOperator op, int right);
Data num_op(float left, BinaryOperator op, float right);

// std::ostream &operator<<(std::ostream &os, Data const &m);