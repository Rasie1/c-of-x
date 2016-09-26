#include "Operator.h"
#include "Lambda.h"
#include "Identifier.h"
#include "Operation.h"

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

ExpPtr Operator::partialApply(ExpPtrArg e, Environment& env) const
{
    auto id = make_ptr<Identifier>(e->show());
    auto th = s_cast<const Operator>(shared_from_this());
    auto body = make_ptr<Operation>(std::const_pointer_cast<Operator>(th), e, id);

    auto closure = Lambda().operate(id, body, env);
    return closure;
}

bool Operator::unapplyVariables(ExpPtrArg e, ExpPtrArg l, ExpPtrArg r, Environment &env) const
{
    return false;
}
