#include "Quote.h"
#include "Application.h"
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
    if (typeid(*(this->e)) == typeid(Operation) &&
        typeid(*e)         == typeid(Operation))
    {
        auto l = s_cast<Operation>(this->e);
        auto r = s_cast<Operation>(e);

        auto rl = Identifier::unwrapIfId(r->left, env);
        if (typeid(*rl) == typeid(Quote))
        {
            if (typeid(*(r->right)) == typeid(Operation))
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
