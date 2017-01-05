#include "Quote.h"
#include "Expressions/Application.h"
#include "Operation.h"
#include "Identifier.h"

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
    if (checkType<Operation>(this->e) && checkType<Operation>(e))
    {
        auto l = s_cast<Operation>(this->e);
        auto r = s_cast<Operation>(e);

        auto rl = Identifier::unwrapIfId(r->left, env);
        if (checkType<Quote>(rl))
        {
            if (checkType<Operation>(r->right))
                r = s_cast<Operation>(r->right);
            else
                return this->e->unapplyVariables(r, env);
        }

        if (*(l->op) != *(r->op))
            return false;

        return l->left->unapplyVariables(r->left, env) &&
               l->right->unapplyVariables(r->right, env);
    }
    else
        return this->e->unapplyVariables(e, env);
}
