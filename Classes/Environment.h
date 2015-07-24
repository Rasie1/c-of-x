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
typedef std::shared_ptr<Expression> ExpPtr;
typedef std::shared_ptr<Pattern>    PatPtr;
class Environment
{
public:
    Environment(EnvironmentalVariable* data, Environment* next = nullptr);
    ~Environment();

    void clear();
    Environment* add(const ExpPtr& p,
                     const ExpPtr& e);
    Environment* pop();
    ExpPtr get(const ExpPtr& p);
    std::pair<ExpPtr, ExpPtr> top();
    bool compareOperators(const ExpPtr& first, const ExpPtr& second);
    Environment* getNext();

    static Environment* create();

private:
    Environment* loadDefaultVariables();

    EnvironmentalVariable* data;
    Environment* next;
    //PrecedenceTable* precedenceTable;
};

/*
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
*/
}
