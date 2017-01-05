#pragma once
#include "Expressions/Expression.h"
#include "Expressions/Function.h"
#include "System/Environment.h"

class Environment;
class Closure : public Function
{
public:
    Closure(ExpPtrArg argument,
            ExpPtrArg body,
            const Environment& env);
    ~Closure();

    ExpPtr apply(ExpPtrArg e, Environment& env) const override;
    std::string show() const override;
    bool operator==(const Expression& other) const override;

    ExpPtr body;
    ExpPtr argument;

    Environment env;
    int envSize;
};

