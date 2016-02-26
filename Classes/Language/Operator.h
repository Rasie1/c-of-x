#pragma once
#include "DataType.h"

class Operator : public DataType
{
public:
    Operator(bool isRightAssociative = false, int priority = 5);

    virtual ExpPtr operate(ExpPtrArg first,
                           ExpPtrArg second,
                           Environment& env) const = 0;

    bool isRightAssociative;
    int priority;
    const std::string name;

    virtual bool operator==(const Expression& other) const override;

    virtual bool unwind(ExpPtr& left,
                        ExpPtr& right,
                        ExpPtr& lvalue,
                        ExpPtr& rvalue,
                        Environment& env);

    virtual ExpPtr partialApply(ExpPtrArg e, Environment& env);

    virtual std::string show() const;
private:
};

