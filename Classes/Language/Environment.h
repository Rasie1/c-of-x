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

    std::pair<ExpPtr, Environment*> process(ExpPtrArg id,
                                            const std::shared_ptr<Predicate>& key);

    void clear();
    Environment* add(ExpPtrArg key, ExpPtrArg predicate);
    Environment* addEqual(ExpPtrArg key, ExpPtrArg value);
    Environment* pop();
    ExpPtr getEqual(ExpPtrArg key);
    std::pair<ExpPtr, ExpPtr> top();
    Environment* getNext();

    //Operators info

    bool compareOperators(const std::shared_ptr<Operator>& first,
                          const std::shared_ptr<Operator>& second);


    ExpPtr intersect(ExpPtrArg l, ExpPtrArg r);

    static Environment* create();

private:
    Environment* loadDefaultVariables();

    ExpPtr key;
    ExpPtr value;

    Environment* next;
};
