#pragma once
#include <vector>
#include <map>
#include <unordered_map>
#include <memory>

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
    Environment* add(const std::shared_ptr<Pattern>& p,
                     Expression* e);
    Environment* pop();
    Expression* get(const std::shared_ptr<Pattern>& p);
    std::pair<std::shared_ptr<Pattern>, Expression*> top();
    bool compareOperators(Expression* first, Expression* second);

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
