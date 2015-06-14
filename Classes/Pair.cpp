#include "Pair.h"
#include "Void.h"

namespace Language
{

Expression* Pair::eval(Environment*& env)
{
    auto ret = l->eval(env);
    auto null = dynamic_cast<Void*>(r);

    // Returns if right value is void
    if (null != nullptr)
        return ret;

    ret = r->eval(env);

    return ret;
}

std::string Pair::toString() const
{
    return "(" + l->toString() + ":" + r->toString() + ")";
}


Expression* PairConstructorWithValue::applyConstEnv(Expression* e, Environment* env)
{
    return new Pair(value->eval(env), e->eval(env));
}

Expression* PairConstructorWithValue::apply(Expression* e, Environment*& env)
{
    return new Pair(value->eval(env), e->eval(env));
}

std::string PairConstructorWithValue::toString() const
{
    return "pair{" + value->toString() + "}";
}

Expression* PairConstructor::applyConstEnv(Expression* e, Environment* env)
{
    return new PairConstructorWithValue(e);
}

std::string PairConstructor::toString() const
{
    return defaultName;
}

const std::string PairConstructor::defaultName = "pair";

}
