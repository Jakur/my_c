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

// class Data
// {
// public:
//     virtual std::string to_string() = 0; // All types can be made strings
//     virtual std::variant<int, float, char, bool, std::string> data() = 0;
//     std::optional<int> to_int()
//     {
//         return {};
//     }
//     std::optional<float> to_float()
//     {
//         return {};
//     }
//     std::optional<char> to_char()
//     {
//         return {};
//     }
//     std::optional<bool> to_bool()
//     {
//         return {};
//     }
// };

// class Array
// {
// };

// class Int : Data
// {
// public:
//     int value;
//     Int(int value);
//     std::variant<int, float, char, bool, std::string> data();
//     std::string to_string();
//     std::optional<int> to_int();
// };