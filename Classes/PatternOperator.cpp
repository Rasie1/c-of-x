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

bool PatternOperator::match(Pattern* other, Environment* env)
{
    auto y = dynamic_cast<PatternVariable*>(other);
    if (y)
        return y->name == name;
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
