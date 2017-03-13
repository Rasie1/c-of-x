#include "Expressions/Include.h"
#include "Expressions/String.h"
#include "Parser/Parser.h"
#include "System/Environment.h"
#include "Expressions/Error.h"
#include "System/Cast.h"

ExpPtr Include::apply(ExpPtrArg e, Environment& env)
{
    auto str = d_cast<String>(e);
    if (!str)
        return make_ptr<ErrorWithMessage>("Not a string");

    auto filename = str->value;

    Parser parser;

    return parser.parseFile(filename, env);
}

std::string Include::show() const
{
    return defaultName;
}

const std::string Include::defaultName = "include";

