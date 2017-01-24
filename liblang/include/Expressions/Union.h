#pragma once
#include "Expressions/Operator.h"
#include "Expressions/Operation.h"
#include "Expressions/Void.h"

class Union : public Operator
{
public:
    Union();

    ExpPtr operate(ExpPtrArg first,
                   ExpPtrArg second,
                   Environment& env) const override;

    std::string show() const override;

    bool unapplyVariables(ExpPtrArg e, ExpPtrArg l, ExpPtrArg r, Environment &env) const override;

    template<class It>
    static ExpPtr make(const It& begin, const It& end)
    {
        auto next = std::next(begin);
        if (next == end)
            return *begin;
//        if (checkType<Void>(next))
//        {

//        }
        if (checkType<Void>(*begin))
        {
            return make(next, end);
        }
        return make_ptr<Operation>(make_ptr<Union>(),
                                   *begin,
                                   make(next, end));
    }

    static const std::string defaultName;
};
