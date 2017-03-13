#pragma once
#include "Expressions/Morphism.h"

class Operator;
class Operation : public Morphism
{
public:
    Operation(const std::shared_ptr<Operator>& op,
              ExpPtrArg left,
              ExpPtrArg right);

    ExpPtr eval(Environment& env) const override;
    std::string show() const override;

    ExpPtr apply(ExpPtrArg other, Environment& env) override;
    ExpPtr intersect(ExpPtrArg other, const Environment& env) override;
    bool hasFreeVariables(const Environment& env) const;
    bool unapplyVariables(ExpPtrArg e, Environment& env) override;

    bool operator==(const Expression& other) const override;

    const std::shared_ptr<Operator> op;
    ExpPtr left;
    ExpPtr right;
};

