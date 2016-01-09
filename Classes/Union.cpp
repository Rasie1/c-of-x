#include "Union.h"
#include "Operation.h"
#include "Void.h"

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

    if (d_cast<Void>(l))
        return r;
    if (d_cast<Void>(r))
        return l;

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
