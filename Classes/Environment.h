#pragma once
#include <vector>
#include <map>
#include <unordered_map>
#include <memory>
#include "Expression.h"

class PrecedenceTable;
class PrecedenceInfo;
class Operator;
class Environment
{
    typedef std::map<std::string, ExpPtr> Variables;
    typedef std::map<std::string, ExpPtr> Operators;
public:
    Environment(ExpPtrArg key, ExpPtrArg value, Environment* next = nullptr);

    void clear();
    Environment* add(ExpPtrArg key,
                     ExpPtrArg value);
    Environment* pop();
    ExpPtr get(ExpPtrArg key);
    std::pair<ExpPtr, ExpPtr> top();
    bool compareOperators(ExpPtrArg first, ExpPtrArg second);
    Environment* getNext();

    static Environment* create();

private:
    Environment* loadDefaultVariables();

    Variables variables;
    Operators operators;

    ExpPtr lookup(const std::string& name);

    ExpPtr key;
    ExpPtr value;

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

