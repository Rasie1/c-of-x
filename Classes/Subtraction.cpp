#include "Subtraction.h"
#include <string>

namespace Language
{

std::string SubtractionOfValue::toString() const
{
    return std::to_string(value) + " -";
}

Expression* SubtractionOfValue::apply(Expression* e, Environment* env)
{
    e = e->eval(env);
    {
        auto i = dynamic_cast<Integer*>(e);
        if (i)
            return new Integer(i->value - value);
    }
    return Expression::apply(e, env);
}

Expression* Subtraction::apply(Expression* e, Environment* env)
{
    e = e->eval(env);
    {
        auto i = dynamic_cast<Integer*>(e);
        if (i)
            return new SubtractionOfValue(i->value);
    }
    return Expression::apply(e, env);
}

std::string Subtraction::toString() const
{
    return "-";
}


}
