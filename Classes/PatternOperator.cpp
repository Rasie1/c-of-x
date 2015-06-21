#include "PatternOperator.h"
#include "PatternVariable.h"
#include "Operator.h"

namespace Language
{

PatternOperator::PatternOperator(const std::string& name,
                                 Operator* op)
    : name(name),
      op(op)
{

}

bool PatternOperator::match(const std::shared_ptr<Pattern>& other,
                            Environment* env) const
{
    auto y = std::dynamic_pointer_cast<PatternVariable>(other);
    if (y)
        return y->name == name;
    auto x = std::dynamic_pointer_cast<PatternOperator>(other);
    if (x)
        return false;
    return x->name == name;
}

bool PatternOperator::isOperator(Environment* env)
{
    return true;
}

std::string PatternOperator::toString() const
{
    return "operator";
}

bool PatternOperator::isMoreThan(const std::shared_ptr<PatternOperator>& other,
                                 Environment* env) const
{
    // ???
    return false;
}

}
