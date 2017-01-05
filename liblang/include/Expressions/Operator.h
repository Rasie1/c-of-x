#pragma once
#include "Expressions/DataType.h"

class Operator : public Data
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

    virtual bool unapplyVariables(ExpPtrArg e, ExpPtrArg l, ExpPtrArg r, Environment &env) const;

    virtual ExpPtr partialApply(ExpPtrArg e, Environment& env) const;

    virtual std::string show() const;
private:
};

