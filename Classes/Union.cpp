#include "Union.h"
#include "Operation.h"
#include "Void.h"

Union::Union()
    : Operator(false, 4)
{
}

bool findSameOperand(ExpPtrArg where, ExpPtrArg what)
{
    if (where->match(what))
        return true;
    auto op = d_cast<Operation>(where);
    if (op && d_cast<Union>(op->op))
            return findSameOperand(op->left, what) ||
                   findSameOperand(op->right, what);
    return false;
}

ExpPtr Union::operate(ExpPtrArg first,
                      ExpPtrArg second,
                      Environment*& env) const
{
    auto l = first->eval(env);
    auto r = second->eval(env);

    if (d_cast<Void>(l) || findSameOperand(r, l))
        return r;
    if (d_cast<Void>(r) || findSameOperand(l, r))
        return l;

    return make_ptr<Operation>(make_ptr<Union>(), l, r);
}

std::string Union::show() const
{
    return Union::defaultName;
}

const std::string Union::defaultName = "|";
