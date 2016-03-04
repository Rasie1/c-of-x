#include "Quote.h"
#include "Application.h"

ExpPtr Quote::apply(ExpPtrArg e, Environment& env) const
{
    return make_ptr<QuotedExpression>(e);
}

std::string Quote::show() const
{
    return defaultName;
}

const std::string Quote::defaultName = "\'";


std::string QuotedExpression::show() const
{
    return defaultName + "(" + e->show() + ")";
}

const std::string QuotedExpression::defaultName = "\'";

bool QuotedExpression::unapplyVariables(ExpPtrArg e, Environment& env) const
{
    return this->e->unapplyVariables(e, env);
}
