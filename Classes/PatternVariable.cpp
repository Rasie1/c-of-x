#include "PatternVariable.h"
#include "PatternOperator.h"
#include "Application.h"
#include "Environment.h"

namespace Language
{

bool PatternVariable::match(ExpPtrArg other,
                            Environment* env) const
{

    //auto y = std::dynamic_pointer_cast<PatternVariable>(other);
    //if (y)
    //    return y->name == name;
    //auto x = std::dynamic_pointer_cast<PatternOperator>(other);
    //if (x)
    //    return x->name == name;
    return true;
    /*
    auto x = dynamic_cast<PatternVariable*>(other);
    if (x == nullptr)
        return false;
    return x->name == name;*/
}


std::string PatternVariable::toString() const
{
    return "auto " + name;
}

}
