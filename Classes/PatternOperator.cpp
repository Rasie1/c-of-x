#include "PatternOperator.h"
#include "Operator.h"

namespace Language
{

PatternOperator::PatternOperator(const std::string& name,
                                 Operator* op)
    : name(name),
      op(op)
{

}

bool PatternOperator::match(Pattern* other, Environment* env)
{
    auto x = dynamic_cast<PatternOperator*>(other);
    if (x == nullptr)
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

bool PatternOperator::isMoreThan(PatternOperator* other,
                                 Environment* env) const
{
    // ???
    return false;
}

}
