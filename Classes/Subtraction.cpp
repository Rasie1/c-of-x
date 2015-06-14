#include "Subtraction.h"
#include <string>

namespace Language
{

std::string SubtractionOfValue::toString() const
{
    return std::to_string(value) + " -";
}

Expression* SubtractionOfValue::applyConstEnv(Expression* e, Environment* env)
{
    e = e->evalConstEnv(env);
    {
        auto i = dynamic_cast<Integer*>(e);
        if (i)
            return new Integer(i->value - value);
    }
    return Expression::applyConstEnv(e, env);
}

Expression* Subtraction::applyConstEnv(Expression* e, Environment* env)
{
    e = e->evalConstEnv(env);
    {
        auto i = dynamic_cast<Integer*>(e);
        if (i)
            return new SubtractionOfValue(i->value);
    }
    return Expression::applyConstEnv(e, env);
}


std::string Subtraction::toString() const
{
    return defaultName;
}

const std::string Subtraction::defaultName = "subtract";



}
