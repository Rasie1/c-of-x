#pragma once
#include "Operator.h"

namespace Language
{

class Application : public Operator
{
public:
    Application();

    ExpPtr operate(const ExpPtr&  first,
                   const ExpPtr&  second,
                   Environment*& env) const override;
    PatPtr leftPattern(const ExpPtr& e) const override;
    PatPtr rightPattern(const ExpPtr& e) const override;
    PatPtr pattern() const override;

    std::string toString() const override;
private:
};

}
