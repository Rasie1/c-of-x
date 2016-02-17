#pragma once
#include "Expression.h"

class Operator;
class Identifier;
class Function;
class Predicate;
class Environment
{
public:
    Environment(ExpPtrArg key, ExpPtrArg value, Environment* next = nullptr);

    void clear();
    Environment* add(ExpPtrArg key, ExpPtrArg predicate);
    Environment* addEqual(ExpPtrArg key, ExpPtrArg value);
    ExpPtr get(ExpPtrArg key);
    ExpPtr getEqual(ExpPtrArg key);
    ExpPtr intersect(ExpPtrArg l, ExpPtrArg r);

    //Operators info

    bool compareOperators(const std::shared_ptr<Operator>& first,
                          const std::shared_ptr<Operator>& second);


    static Environment* create();

private:
    Environment* loadDefaultVariables();

    ExpPtr key;
    ExpPtr value;

    Environment* next;
};
