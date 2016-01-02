#pragma once
#include "Expression.h"

class Function;
class Environment;
class Closure : public Expression
{
public:
    Closure(const std::shared_ptr<Function>& function,
            Environment* env,
            int envSize = 0);
    ~Closure();

    ExpPtr apply(ExpPtrArg e, Environment*& env) const override;
    std::string show() const override;

private:
    std::shared_ptr<Function> function;
    Environment* env;
    int envSize;
};

