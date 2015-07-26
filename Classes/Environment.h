#pragma once
#include <vector>
#include <map>
#include <unordered_map>
#include <memory>
#include "Expression.h"

namespace Language
{

class EnvironmentalVariable;
class PrecedenceTable;
class PrecedenceInfo;
class Operator;
class Environment
{
    typedef std::map<std::string, ExpPtr> Variables;
    typedef std::map<std::string, ExpPtr> Operators;
public:
    Environment(EnvironmentalVariable* data, Environment* next = nullptr);
    ~Environment();

    void clear();
    Environment* add(ExpPtrArg p,
                     ExpPtrArg e);
    Environment* pop();
    ExpPtr get(ExpPtrArg p);
    std::pair<ExpPtr, ExpPtr> top();
    bool compareOperators(ExpPtrArg first, ExpPtrArg second);
    Environment* getNext();

    static Environment* create();

private:
    Environment* loadDefaultVariables();

    Variables variables;
    Operators operators;

    ExpPtr lookup(const std::string& name);

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
