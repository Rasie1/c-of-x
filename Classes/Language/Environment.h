#pragma once
#include "Expression.h"

class Operator;
class Identifier;
class Function;
class Environment
{
public:
    Environment(ExpPtrArg key, ExpPtrArg value, Environment* next = nullptr);

    std::pair<ExpPtr, Environment*> process(ExpPtrArg id, ExpPtrArg key);

    void clear();
    Environment* add(ExpPtrArg key,
                     ExpPtrArg value);
    Environment* pop();
    ExpPtr get(ExpPtrArg key);
    std::pair<ExpPtr, ExpPtr> top();
    bool compareOperators(const std::shared_ptr<Operator>& first,
                          const std::shared_ptr<Operator>& second);
    Environment* getNext();

    static Environment* create();

private:
    Environment* loadDefaultVariables();

    ExpPtr key;
    ExpPtr value;

    Environment* next;
};
