#pragma once
#include "Expressions/Morphism.h"

class Operator : public Morphism
{
public:
    Operator(bool isRightAssociative = false, int priority = 5, bool splitting = false);

    virtual ExpPtr operate(ExpPtrArg first,
                           ExpPtrArg second,
                           Environment& env) const = 0;

    bool isRightAssociative;
    int priority;
    const std::string name;
    bool splitting;

    bool operator==(const Expression& other) const override;

    virtual ExpPtr intersect(ExpPtrArg l, ExpPtrArg r, ExpPtrArg e, Environment &env) const;
    virtual bool unapplyVariables(ExpPtrArg e, ExpPtrArg l, ExpPtrArg r, Environment &env) const;

    virtual ExpPtr partialApplyLeft  (ExpPtrArg e, Environment& env) const;
    virtual ExpPtr partialApplyRight (ExpPtrArg e, Environment& env) const;
    virtual ExpPtr partialApplyNoArgs(Environment& env) const;

    ExpPtr apply(ExpPtrArg e, Environment& env) const override;

    virtual std::string show() const;
private:
};

