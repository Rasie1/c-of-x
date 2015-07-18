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
    PatPtr leftPattern (const ExpPtr& e) const override;
    PatPtr rightPattern(const ExpPtr& e) const override;
    PatPtr pattern() const override;

    virtual bool unwind(const ExpPtr& left,
                        const ExpPtr& right,
                        ExpPtr& lvalue,
                        ExpPtr& rvalue,
                        Environment*& env) override;

    std::string toString() const override;
private:
};

}
