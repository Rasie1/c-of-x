#include "Addition.h"
#include <string>

namespace Language
{

std::string AdditionOfValue::toString() const
{
    return std::to_string(value) + " +";
}

Expression* AdditionOfValue::applyConstEnv(Expression* e, Environment* env)
{
    e = e->evalConstEnv(env);
    {
        auto i = dynamic_cast<Integer*>(e);
        if (i)
            return new Integer(i->value + value);
    }
    return Expression::applyConstEnv(e, env);
}

Expression* Addition::applyConstEnv(Expression* e, Environment* env)
{
    e = e->evalConstEnv(env);
    {
        auto i = dynamic_cast<Integer*>(e);
        if (i)
            return new AdditionOfValue(i->value);
    }
    return Expression::applyConstEnv(e, env);
}

std::string Addition::toString() const
{
    return defaultName;
}

const std::string Addition::defaultName = "add";


}
