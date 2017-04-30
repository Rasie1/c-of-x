#include "Expressions/Operation.h"
#include "Expressions/Operator.h"
#include "Expressions/Union.h"
#include "Expressions/Identifier.h"
#include "System/Environment.h"
#include "Expressions/Any.h"
#include "Expressions/Intersection.h"
#include "System/Cast.h"
#include <memory>

Operation::Operation(const std::shared_ptr<Operator>& op,
                     const Object& left,
                     const Object& right)
    : op(op),
      left(left),
      right(right)
{

}

Object Operation::eval(Environment& env)
{
    Object ret;

    env.debugPrint("OPERATION: " + this->op->show() + '\n', true);

    env.debugPrint("FST: " + this->left->show() + '\n', true);
    env.debugPrint("SND: " + this->right->show() + '\n', true);


    env.increaseDebugIndentation();
    ret = op->operate(left, right, env);
    env.decreaseDebugIndentation();

    // preserve identity
    if (auto operation = std::dynamic_pointer_cast<Operation>(ret.expression))
    if (typeid(*(operation->op)) == typeid(*(this->op)))
    if (operation->left == left)
    if (operation->right == right)
    {
        env.debugPrint("identity restored\n");
        ret = thisObject();
    }


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

Object Operation::intersect(const Object& other, Environment& envc)
{
    auto env = envc;
    return op->intersect(left, right, other, env);
}

Object Operation::element(const Environment& envc)
{
    auto env = envc;
    return op->element(left, right, env);
}

Object Operation::apply(const Object& other, Environment& env)
{
    return op->apply(left, right, other, env);
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

bool Operation::unapplyVariables(const Object& e, Environment& env)
{
    return op->unapplyVariables(e, this->left, this->right, env);
}
