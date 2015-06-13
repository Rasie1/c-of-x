#include "Pair.h"
#include "Void.h"

namespace Language
{

Expression* Pair::eval(Environment* env)
{
    auto ret = l->evalModifyEnv(env);
    auto null = dynamic_cast<Void*>(r);

    if (null == nullptr)
        return ret;

    ret = r->eval(env);

    return ret;
}

std::string Pair::toString() const
{
    return "(" + l->toString() + ":" + r->toString() + ")";
}


Expression* PairConstructorWithValue::eval(Environment* env)
{
    return this;
}

Expression* PairConstructorWithValue::apply(Expression* e, Environment* env)
{
    return new Pair(value, e);
}

std::string PairConstructorWithValue::toString() const
{
    return "pair " + value->toString();
}


Expression* PairConstructor::eval(Environment* env)
{
    return this;
}

Expression* PairConstructor::apply(Expression* e, Environment* env)
{
    return new PairConstructorWithValue(e);
}

std::string PairConstructor::toString() const
{
    return defaultName;
}

const std::string PairConstructor::defaultName = "pair";

}
