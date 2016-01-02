#include "Include.h"
#include "String.h"
#include "Parser.h"
#include "Environment.h"

ExpPtr Include::apply(ExpPtrArg e, Environment*& env) const
{
    auto str = d_cast<String>(e);
    auto newEnv = env;
    if (str == nullptr)
        return Expression::apply(e, newEnv);

    auto filename = str->value;

    return Parser::parseFile(filename, env);
}

std::string Include::show() const
{
    return defaultName;
}

const std::string Include::defaultName = "include";

