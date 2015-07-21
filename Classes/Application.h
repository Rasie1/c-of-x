#pragma once
#include "Operator.h"

namespace Language
{

class Application : public Operator
{
public:
    Application();

    ExpPtr operate(const ExpPtr& first,
                   const ExpPtr& second,
                   Environment*& env) const override;
    PatPtr pattern() const override;

    virtual bool unwind(ExpPtr& left,
                        ExpPtr& right,
                        ExpPtr& lvalue,
                        ExpPtr& rvalue,
                        Environment*& env) override;

    std::string toString() const override;
private:
};

}
