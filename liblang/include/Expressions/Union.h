#pragma once
#include "Expressions/Operator.h"
#include "Expressions/Operation.h"
#include "Expressions/Void.h"
#include <vector>

class Union : public Operator
{
public:
    Union();

    ExpPtr operate(ExpPtrArg first,
                   ExpPtrArg second,
                   Environment& env) override;

    std::string show() const override;


    ExpPtr intersect(ExpPtrArg l, ExpPtrArg r, ExpPtrArg e, Environment &env) override;
    bool unapplyVariables(ExpPtrArg e, ExpPtrArg l, ExpPtrArg r, Environment &env) override;

    static ExpPtr make(const std::vector<ExpPtr>::iterator& begin, 
                       const std::vector<ExpPtr>::iterator& end);

    static const std::string defaultName;
};
