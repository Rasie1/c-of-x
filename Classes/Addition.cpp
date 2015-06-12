#include "Addition.h"
#include <string>

namespace Language
{

std::string AdditionOfValue::toString() const
{
    return std::to_string(value) + " +";
}

Expression* AdditionOfValue::apply(Expression* e, Environment* env)
{
    e = e->eval(env);
    {
        auto i = dynamic_cast<Integer*>(e);
        if (i)
            return new Integer(i->value + value);
    }
    return Expression::apply(e, env);
}

Expression* Addition::apply(Expression* e, Environment* env)
{
    e = e->eval(env);
    {
        auto i = dynamic_cast<Integer*>(e);
        if (i)
            return new AdditionOfValue(i->value);
    }
    return Expression::apply(e, env);
}

std::string Addition::toString() const
{
    return "+";
}


}
