#pragma once
#include "Operator.h"
#include "Operation.h"

class Union : public Operator
{
public:
    Union();

    ExpPtr operate(ExpPtrArg first,
                   ExpPtrArg second,
                   Environment& env) const override;

    std::string show() const override;

    template<class It>
    static ExpPtr make(const It& begin, const It& end)
    {
        if (std::next(begin) == end)
            return *begin;
        return make_ptr<Operation>(make_ptr<Union>(),
                                   *begin,
                                   make(std::next(begin), end));
    }

    static const std::string defaultName;
};
