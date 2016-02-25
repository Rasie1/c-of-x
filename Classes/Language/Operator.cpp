#include "Operator.h"
#include <string>

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
