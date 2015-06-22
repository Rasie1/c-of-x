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

    std::string toString() const override;
private:
};

}
