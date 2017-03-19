#pragma once
#include "Expressions/Morphism.h"

class Operator;
class Operation : public Morphism
{
public:
    Operation(const std::shared_ptr<Operator>& op,
              const Object& left,
              const Object& right);

    Object eval(Environment& env) const override;
    std::string show() const override;

    Object apply(const Object& other, Environment& env) override;
    Object intersect(const Object& other, const Environment& env) override;
    bool hasFreeVariables(const Environment& env) const;
    bool unapplyVariables(const Object& e, Environment& env) override;

    bool operator==(const Expression& other) const override;

    const std::shared_ptr<Operator> op;
    Object left;
    Object right;
};

