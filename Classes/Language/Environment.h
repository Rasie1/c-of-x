#pragma once
#include "Expression.h"
#include <unordered_map>

class Operator;
class Environment
{
public:
    Environment();

    void add(ExpPtrArg key, ExpPtrArg predicate);
    void addEqual(ExpPtrArg key, ExpPtrArg value);
    ExpPtr get(ExpPtrArg key);
    ExpPtr getEqual(ExpPtrArg key);
    ExpPtr intersect(ExpPtrArg l, ExpPtrArg r);
    void clear();

    //Operators info

    bool compareOperators(const std::shared_ptr<Operator>& first,
                          const std::shared_ptr<Operator>& second);

private:
    void addDefaultVariables();

    std::unordered_map<Expression, ExpPtr> data;
};
