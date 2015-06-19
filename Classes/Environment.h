#pragma once
#include <vector>
#include <map>
#include <unordered_map>

namespace Language
{

class Expression;
class EnvironmentalVariable;
class Pattern;
class PrecedenceTable;
class PrecedenceInfo;
class Operator;
class Environment
{
public:
    Environment(EnvironmentalVariable* data, Environment* next = nullptr);
    ~Environment();

    void clean();
    void clear();
    Environment* add(Pattern* p, Expression* e);
    Environment* pop();
    Expression* get(Pattern* p);

    static Environment* create();

private:
    Environment* loadDefaultVariables();

    EnvironmentalVariable* data;
    Environment* next;
    PrecedenceTable* precedenceTable;
};


class PrecedenceTable
{
public:
    PrecedenceTable() {}
    bool more(Operator* first, Operator* second);
    void add(PrecedenceInfo* op);
    void remove(PrecedenceInfo* op);

private:
    std::unordered_map<unsigned,
                       std::map<double, double>> table;
};

}
