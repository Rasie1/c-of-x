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

    env.debugPrint("OPERATION: " + this->op->show() + '\n', true);

    env.debugPrint("SND: " + this->right->show() + '\n', true);
    env.debugPrint("FST: " + this->left->show() + '\n', true);


    env.increaseDebugIndentation();
    ret = op->operate(left, right, env);
    env.decreaseDebugIndentation();

    env.debugPrint("RES: " + ret->show() + '\n', true);

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