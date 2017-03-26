#include "Expressions/Quote.h"
#include "Expressions/Application.h"
#include "Expressions/Operation.h"
#include "Expressions/Identifier.h"
#include "System/Cast.h"

Object Quote::apply(const Object& e, Environment& env)
{
    return makeObject<QuotedExpression>(e);
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

bool QuotedExpression::unapplyVariables(const Object& e, Environment& env)
{
    if (auto opThis = cast<Operation>(env, this->e))
    {
        if (auto opOther = cast<Operation>(env, e))
        {
            if (*(opThis->op) == *(opOther->op))
            {
                auto lUnapplied = opThis->left->unapplyVariables(opOther->left, env);
                auto rUnapplied = opThis->right->unapplyVariables(opOther->right, env);

                return lUnapplied && rUnapplied;
            }
        }
    }

    return false;// this->e->unapplyVariables(e, env);
//    if (checkType<Operation>(env, this->e) && checkType<Operation>(e))
//    {
//        auto l = s_cast<Operation>(env, this->e);
//        auto r = s_cast<Operation>(env, e);

//        auto rl = Identifier::unwrapIfId(r->left, env);
//        if (checkType<Quote>(env, rl))
//        {
//            if (checkType<Operation>(env, r->right))
//                r = s_cast<Operation>(env, r->right);
//            else
//                return this->e->unapplyVariables(r, env);
//        }

//        if (*(l->op) != *(r->op))
//            return false;

//        return l->left->unapplyVariables(r->left, env) &&
//               l->right->unapplyVariables(r->right, env);
//    }
//    else
//        return this->e->unapplyVariables(e, env);
}
