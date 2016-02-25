#pragma once
#include "Operator.h"
#include <vector>

class Union : public Operator
{
public:
    Union();

    ExpPtr operate(ExpPtrArg first,
                   ExpPtrArg second,
                   Environment& env) const override;

    std::string show() const override;

    static ExpPtr make(const std::vector<ExpPtr>& xs);

    static const std::string defaultName;
};
