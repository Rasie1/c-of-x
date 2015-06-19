#include "Addition.h"
#include <string>
#include "Operation.h"

namespace Language
{

std::string AdditionOfValue::toString() const
{
    return std::to_string(value) + " +";
}

Expression* AdditionOfValue::apply(Expression* e, Environment*& env)
{
    e = e->eval(env);
    {
        auto i = dynamic_cast<Integer*>(e);
        if (i)
            return new Integer(i->value + value);
    }
    return Expression::apply(e, env);
}

Expression* Addition::apply(Expression* e, Environment*& env)
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
    return defaultName;
}

const std::string Addition::defaultName = "add";


Expression* Plus::operate(Expression* first,
                                      Expression* second,
                                      Environment*& env)
{
    auto firstInteger = dynamic_cast<Integer*>(first);
    auto secondInteger = dynamic_cast<Integer*>(second);

    if (firstInteger && secondInteger)
        return new Integer(firstInteger->value +
                           secondInteger->value);
    return new Operation(new Plus(), first, second);
}

std::string Plus::toString() const
{
    return Plus::name;
}

const std::string Plus::defaultName = "+";

}
