#pragma once
#include "Expression.h"

class Operator;
class Operation : public Expression
{
public:
    Operation(const std::shared_ptr<Operator>& op,
              ExpPtrArg left,
              ExpPtrArg right);

    ExpPtr eval(Environment& env) const override;
    std::string show() const override;

    ExpPtr intersect(ExpPtrArg other, const Environment& env);
    bool hasFreeVariables(const Environment& env) const;
    bool unapplyVariables(ExpPtrArg e, Environment& env) const override;

    bool operator==(const Expression& other) const override;

    std::shared_ptr<Operator> op;
    ExpPtr left;
    ExpPtr right;
};

