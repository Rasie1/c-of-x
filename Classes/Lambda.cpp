#include "Lambda.h"
#include <iostream>
#include "UnknownName.h"
#include "Pattern.h"
#include "Closure.h"
#include "Function.h"
#include "Environment.h"

namespace Language
{

Expression* Lambda::apply(Expression* e, Environment*& env)
{
    return new LambdaArguments(e->pattern());
}

std::string Lambda::toString() const
{
    return defaultName;
}

const std::string Lambda::defaultName = "\\";


LambdaArguments::LambdaArguments(const std::shared_ptr<Pattern>& argument)
    : pattern(argument)
{

}

Expression* LambdaArguments::apply(Expression* e, Environment*& env)
{
    auto ret = new Closure(new Function(e, pattern), env);
    return ret;
}

std::string LambdaArguments::toString() const
{
    return "\u03BB" + pattern->toString() + ".";
}

}
