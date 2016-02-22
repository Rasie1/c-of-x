#pragma once
#include "Expression.h"
#include "Function.h"

class Environment;
class Closure : public Function
{
public:
    Closure(ExpPtrArg argument,
            ExpPtrArg body,
            Environment* env,
            int envSize = 0);
    ~Closure();

    ExpPtr apply(ExpPtrArg e, Environment*& env) const override;
    std::string show() const override;
    bool operator==(const Expression& other) const override;

    ExpPtr body;
    ExpPtr argument;

    Environment* env;
    int envSize;
};

