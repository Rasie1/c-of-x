#include "Include.h"
#include "String.h"
#include "Parser.h"
#include "Environment.h"

namespace Language
{

ExpPtr Include::apply(ExpPtrArg e, Environment*& env) const
{
    auto str = std::dynamic_pointer_cast<String>(e);
    auto newEnv = env;
    if (str == nullptr)
        return Expression::apply(e, newEnv);

    auto filename = str->value;

    return Parser::parseFile(filename, env);
}

std::string Include::toString() const
{
    return defaultName;
}

const std::string Include::defaultName = "include";

}
