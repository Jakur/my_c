#include <string>
#include "stdio.h"
#include "iostream"
#include <map>
#include <vector>

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

void print_op(BinaryOperator op);

class Array;

class Data
{
public:
    enum
    {
        INT,
        CHAR,
        BOOL,
        FLOAT,
        STRING,
        ARR,
    } tag;
    union {
        int i;
        char c;
        float f;
        bool b;
        std::string *s;
        Array *a;
    };
    Data(int i);
    Data(float f);
    Data(bool b);
    Data(char c);
    Data(std::string *s);
    Data(Array *a);
    Data();
    Data apply(Data *other, BinaryOperator op);
    void print();
};

Data bool_op(bool left, BinaryOperator op, bool right);
Data num_op(int left, BinaryOperator op, int right);
Data num_op(float left, BinaryOperator op, float right);
Data str_op(std::string *left, BinaryOperator op, std::string *right);

class Array
{
public:
    int total_size;
    std::vector<int> sizes;
    std::vector<Data> data;
    Array(int total_size, std::vector<int> sizes) : total_size{total_size}, sizes{sizes}, data{std::vector<Data>(total_size, Data(0))} {}
    int one_d(std::vector<int> indices);
    Data get(std::vector<int> indices);
    Data set(std::vector<int> indices, Data val);
    Data set(int index, Data val);
    void print();
};

class VarStorage
{
protected:
    std::map<std::string, Data> state;

public:
    VarStorage(std::map<std::string, Data> map) : state{map} {}
    void declare(std::string id, Data init);
    void assign(std::string id, Data val);
    Data get(std::string *name);
};