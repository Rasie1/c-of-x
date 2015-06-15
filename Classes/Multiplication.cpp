#include "Multiplication.h"
#include <string>

namespace Language
{

std::string MultiplicationOfValue::toString() const
{
    return std::to_string(value) + " *";
}

Expression* MultiplicationOfValue::apply(Expression* e, Environment*& env)
{
    e = e->evalConstEnv(env);
    {
        auto i = dynamic_cast<Integer*>(e);
        if (i)
            return new Integer(i->value * value);
    }
    return Expression::applyConstEnv(e, env);
}

Expression* Multiplication::apply(Expression* e, Environment*& env)
{
    e = e->evalConstEnv(env);
    {
        auto i = dynamic_cast<Integer*>(e);
        if (i)
            return new MultiplicationOfValue(i->value);
    }
    return Expression::applyConstEnv(e, env);
}

std::string Multiplication::toString() const
{
    return defaultName;
}

const std::string Multiplication::defaultName = "multiply";



}
