#include "Lambda.h"
#include <iostream>
#include "UnknownName.h"
#include "Pattern.h"
#include "Closure.h"
#include "Function.h"
#include "Environment.h"

namespace Language
{

Expression* Lambda::applyConstEnv(Expression* e, Environment* env)
{
    {
        auto i = dynamic_cast<UnknownName*>(e);
        if (i)
            return new LambdaArguments(e->pattern());
    }
    return new LambdaArguments(e->pattern());
}

std::string Lambda::toString() const
{
    return defaultName;
}

const std::string Lambda::defaultName = "\\";


LambdaArguments::LambdaArguments(Pattern* argument)
    : pattern(argument)
{

}

Expression* LambdaArguments::applyConstEnv(Expression* e, Environment* env)
{
    auto ret = new Closure(new Function(e, pattern), env);
    return ret;
}

std::string LambdaArguments::toString() const
{
    return "\\" + pattern->toString() + ".";
}

}
