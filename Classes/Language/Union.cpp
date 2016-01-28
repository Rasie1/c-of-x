#include "Union.h"
#include "Operation.h"
#include "Void.h"
#include "Identifier.h"

Union::Union()
    : Operator(false, 4)
{
}

bool findSameOperand(ExpPtrArg where, ExpPtrArg what)
{
    if (*where == *what)
        return true;
    return false;

//    auto op = d_cast<Operation>(where);
//    if (op && d_cast<Union>(op->op))
//        return findSameOperand(op->left, what) ||
//               findSameOperand(op->right, what);
//    return false;
}

bool isVoid(ExpPtr x)
{
    if (d_cast<Void>(x))
        return true;

    auto id = d_cast<Identifier>(x);

    if (id && id->name == Void::defaultName)
        return true;

    return false;
}

ExpPtr Union::make(const std::vector<ExpPtr>& xs)
{
    // TODO: make generic tree algorithm

    if (xs.size() == 0)
        return make_ptr<Void>();

    if (xs.size() == 1)
        return xs[0];

    if (xs.size() == 2)
    {
        auto l = xs[0];
        auto r = xs[1];
        if (isVoid(l) || findSameOperand(r, l))
            return r;
        if (isVoid(r) || findSameOperand(l, r))
            return l;
        return make_ptr<Operation>(make_ptr<Union>(), l, r);
    }

    if (xs.size() == 3)
    {
        auto l = make({xs[0], xs[1]});
        if (isVoid(xs[2]) ||
            findSameOperand(xs[0], xs[2]) ||
            findSameOperand(xs[1], xs[2]))
            return l;
        return make_ptr<Operation>(make_ptr<Union>(), l, xs[2]);
    }

    if (xs.size() == 4)
    {
        auto l = make({xs[0], xs[1]});
        auto r = make({xs[2], xs[3]});
        return make({l, r});
    }

    if (xs.size() > 4)
        throw "unimplemented";
}

ExpPtr Union::operate(ExpPtrArg first,
                      ExpPtrArg second,
                      Environment*& env) const
{
    auto l = first->eval(env);
    auto r = second->eval(env);

    return make({l, r});
}

std::string Union::show() const
{
    return Union::defaultName;
}

const std::string Union::defaultName = "|";
