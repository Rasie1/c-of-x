#include "Include.h"
#include "String.h"
#include "Parser/Parser.h"
#include "Environment.h"
#include "Error.h"

ExpPtr Include::apply(ExpPtrArg e, Environment& env) const
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

