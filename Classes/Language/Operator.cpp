#include "Operator.h"
#include "Lambda.h"
#include "Identifier.h"
#include "Operation.h"

Operator::Operator(bool isRightAssociative, int priority)
    : isRightAssociative(isRightAssociative),
      priority(priority),
      name("op")
{
}

std::string Operator::show() const
{
    return "op";
}

bool Operator::unwind(ExpPtr& left,
                      ExpPtr& right,
                      ExpPtr& lvalue,
                      ExpPtr& rvalue,
                      Environment& env)
{
    return false;
}

bool Operator::operator==(const Expression& other) const
{
    return false;
//    throw 0; // todo
//    try
//    {
//        auto x = dynamic_cast<const Operator&>(other);
//        return x.op == op && x.left == left && x.right == right;
//    }
//    catch (std::bad_cast&)
//    {
//        return false;
//    }
}

ExpPtr Operator::partialApply(ExpPtrArg e, Environment& env) const
{
    auto id = make_ptr<Identifier>(e->show());
    auto th = s_cast<const Operator>(shared_from_this());
    auto body = make_ptr<Operation>(std::const_pointer_cast<Operator>(th), e, id);

    auto closure = Lambda().operate(id, body, env);
    return closure;
}

void Operator::unapplyVariables(ExpPtrArg e, ExpPtrArg l, ExpPtrArg r, Environment &env) const
{

}
