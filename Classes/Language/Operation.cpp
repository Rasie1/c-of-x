#include "Operation.h"
#include "Operator.h"
#include "Union.h"
#include "Identifier.h"
#include "Environment.h"
#include "Any.h"
#include "Intersection.h"

Operation::Operation(const std::shared_ptr<const Operator>& op,
                     ExpPtrArg left,
                     ExpPtrArg right)
    : op(op),
      left(left),
      right(right)
{

}

ExpPtr Operation::eval(Environment& env) const
{
    ExpPtr ret;

    // debug-only
    DEBUG_PRINT_OP(op);
    auto l = d_cast<Identifier>(left) ? env.getEqual(left) : left;
    if (d_cast<Any>(l))
        l = left;
    auto r = d_cast<Identifier>(right) ? env.getEqual(right) : right;
    if (d_cast<Any>(r))
        r = right;
    DEBUG_PRINT_LT(l);
    DEBUG_PRINT_RT(r);
    // /debug-only

    ret = op->operate(left, right, env);

    DEBUG_PRINT_RS(ret);

    return ret;
}

std::string Operation::show() const
{
    return "(" +
            left->show() +
            " " + op->show() + " " +
            right->show() +
            ")";
}

bool Operation::hasFreeVariables(const Environment& env) const
{
    return left->hasFreeVariables(env) || right->hasFreeVariables(env);
}

ExpPtr Operation::intersect(ExpPtrArg other, const Environment& envc)
{
    auto env = envc;
    return Intersection().operate(Intersection().operate(left, other, env),
                                  Intersection().operate(right, other, env), env);
}

bool Operation::operator==(const Expression& other) const
{
    return typeid(*this) == (typeid(other));
    try
    {
        auto x = dynamic_cast<const Operation&>(other);
        return x.op == op && x.left == left && x.right == right;
    }
    catch (std::bad_cast&)
    {
        return false;
    }
}

bool Operation::unapplyVariables(ExpPtrArg e, Environment& env) const
{
    return op->unapplyVariables(e, this->left, this->right, env);
}
