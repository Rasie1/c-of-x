#include "Expressions/Operator.h"
#include "Expressions/Lambda.h"
#include "Expressions/Void.h"
#include "Expressions/Identifier.h"
#include "Expressions/Intersection.h"
#include "Expressions/Operation.h"
#include "System/Cast.h"

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

Object Operator::partialApplyLeft(const Object& e, Environment& env)
{
    auto arg = make_ptr<Identifier>("arg");
    auto that = std::static_pointer_cast<const Operator>(shared_from_this());
    auto body = make_ptr<Operation>(std::const_pointer_cast<Operator>(that), 
                                    e, 
                                    arg);

    auto closure = Lambda().operate(arg, body, env);
    return closure;
}

Object Operator::partialApplyRight(const Object& e, Environment& env)
{
    auto arg = make_ptr<Identifier>("arg");
    auto that = std::static_pointer_cast<const Operator>(shared_from_this());
    auto body = make_ptr<Operation>(std::const_pointer_cast<Operator>(that), 
                                    arg, 
                                    e);

    auto closure = Lambda().operate(arg, body, env);
    return closure;
}

Object Operator::partialApplyNoArgs(Environment& env)
{
    auto l = make_ptr<Identifier>("l");
    auto r = make_ptr<Identifier>("r");
    auto that = std::static_pointer_cast<const Operator>(shared_from_this());
    auto body = make_ptr<Operation>(std::const_pointer_cast<Operator>(that), 
                                    l, 
                                    r);

    // auto rUnapplied = Lambda().operate(r, body, env);
    auto rUnapplied = make_ptr<Operation>(make_ptr<Lambda>(), r, body);
    auto lUnapplied = Lambda().operate(l, rUnapplied, env);
    return lUnapplied;
}

bool Operator::unapplyVariables(const Object& e, const Object& l, const Object& r, Environment &env)
{
    return false;
}

Object Operator::apply(const Object& e, Environment& env)
{
    return partialApplyRight(e, env);
}

Object Operator::intersect(const Object& l, const Object& r, const Object& e, Environment &env) 
{
    return make_ptr<Operation>(make_ptr<Intersection>(), 
                               operate(l, r, env), 
                               e);
}

Object Operator::apply(const Object& l, const Object& r, const Object& e, Environment &env) 
{
    return make_ptr<Void>();
}
