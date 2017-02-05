#pragma once
#include "Expressions/Operator.h"

class Intersection : public Operator
{
public:
    Intersection();

    ExpPtr operate(ExpPtrArg first,
                   ExpPtrArg second,
                   Environment& env) const override;
    
    virtual ExpPtr apply(ExpPtrArg l, ExpPtrArg r, ExpPtrArg e, Environment &env) const;
    bool unapplyVariables(ExpPtrArg e, ExpPtrArg l, ExpPtrArg r, Environment &env) const override;

    std::string show() const override;

    static const std::string defaultName;
};
