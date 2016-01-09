#include "Union.h"
#include "Operation.h"

Union::Union()
    : Operator(false, 4)
{
}

ExpPtr Union::operate(ExpPtrArg first,
                      ExpPtrArg second,
                      Environment*& env) const
{
    auto l = first->eval(env);
    auto r = second->eval(env);

    if (l->match(r))
        return l;
    else
        return make_ptr<Operation>(make_ptr<Union>(), l, r);
}

std::string Union::show() const
{
    return Union::defaultName;
}

const std::string Union::defaultName = "|";
