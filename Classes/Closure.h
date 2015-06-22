#pragma once
#include "Expression.h"

namespace Language
{

class Function;
class Environment;
class Closure : public Expression
{
public:
    Closure(const std::shared_ptr<Function>& function,
            Environment* env);
    ~Closure();

    ExpPtr apply(const ExpPtr& e, Environment*& env) const override;
    std::string toString() const override;

private:
    std::shared_ptr<Function> function;
    Environment* env;
};

}
