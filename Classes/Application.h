#pragma once
#include "Operator.h"

namespace Language
{

class Application : public Operator
{
public:
    Application();

    Expression* operate(Expression* first,
                        Expression* second,
                        Environment*& env) override;
    std::string toString() const override;
private:
};

}
