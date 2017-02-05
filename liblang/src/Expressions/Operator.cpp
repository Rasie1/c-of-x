#include "Expressions/Operator.h"
#include "Expressions/Lambda.h"
#include "Expressions/Void.h"
#include "Expressions/Identifier.h"
#include "Expressions/Intersection.h"
#include "Expressions/Operation.h"

Operator::Operator(bool isRightAssociative, int priority, bool splitting)
    : isRightAssociative(isRightAssociative),
      priority(priority),
      name("op"),
      splitting(splitting)
{
}

std::string Operator::show() const
{
    return "op";
}

bool Operator::operator==(const Expression& other) const
{
    return typeid(*this) == typeid(other);
}

ExpPtr Operator::partialApplyLeft(ExpPtrArg e, Environment& env) const
{
    auto arg = make_ptr<Identifier>("arg");
    auto that = s_cast<const Operator>(shared_from_this());
    auto body = make_ptr<Operation>(std::const_pointer_cast<Operator>(that), 
                                    e, 
                                    arg);

    auto closure = Lambda().operate(arg, body, env);
    return closure;
}

ExpPtr Operator::partialApplyRight(ExpPtrArg e, Environment& env) const
{
    auto arg = make_ptr<Identifier>("arg");
    auto that = s_cast<const Operator>(shared_from_this());
    auto body = make_ptr<Operation>(std::const_pointer_cast<Operator>(that), 
                                    arg, 
                                    e);

    auto closure = Lambda().operate(arg, body, env);
    return closure;
}

ExpPtr Operator::partialApplyNoArgs(Environment& env) const
{
    auto l = make_ptr<Identifier>("l");
    auto r = make_ptr<Identifier>("r");
    auto that = s_cast<const Operator>(shared_from_this());
    auto body = make_ptr<Operation>(std::const_pointer_cast<Operator>(that), 
                                    l, 
                                    r);

    // auto rUnapplied = Lambda().operate(r, body, env);
    auto rUnapplied = make_ptr<Operation>(make_ptr<Lambda>(), r, body);
    auto lUnapplied = Lambda().operate(l, rUnapplied, env);
    return lUnapplied;
}

bool Operator::unapplyVariables(ExpPtrArg e, ExpPtrArg l, ExpPtrArg r, Environment &env) const
{
    return false;
}

ExpPtr Operator::apply(ExpPtrArg e, Environment& env) const
{
    return partialApplyRight(e, env);
}

ExpPtr Operator::intersect(ExpPtrArg l, ExpPtrArg r, ExpPtrArg e, Environment &env) const 
{
    return make_ptr<Operation>(make_ptr<Intersection>(), operate(l, r, env), e);
}

ExpPtr Operator::apply(ExpPtrArg l, ExpPtrArg r, ExpPtrArg e, Environment &env) const 
{
    return make_ptr<Void>();
}
