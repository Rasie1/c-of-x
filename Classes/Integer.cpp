#include "Integer.h"
#include <string>
#include <algorithm>
#include <iostream>
#include "PatternVariable.h"

Integer::Integer(long long value)
    : value(value)
{
}

bool Integer::match(ExpPtrArg other) const
{
    //auto y = d_cast<PatternVariable>(other);
    //if (y)
    //    return y->name == name;
    //auto x = d_cast<PatternOperator>(other);
    //if (x)
    //    return x->name == name;
    return true;
    /*
    auto x = dynamic_cast<PatternVariable*>(other);
    if (x == nullptr)
        return false;
    return x->name == name;*/
}

std::string Integer::show() const
{
    return std::to_string(value);
}


PatPtr Integer::pattern() const
{
    // Wrong, I think it should return "(== value)?"
    return make_ptr<PatternVariable>(show()); //patternValue
}
