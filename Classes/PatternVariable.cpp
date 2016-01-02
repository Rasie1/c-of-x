#include "PatternVariable.h"
#include "PatternOperator.h"
#include "Application.h"
#include "Environment.h"

namespace Language
{

bool PatternVariable::match(ExpPtrArg other) const
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


std::string PatternVariable::show() const
{
    return "auto " + name;
}

}
