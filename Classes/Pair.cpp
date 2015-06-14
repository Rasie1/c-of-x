#include "Pair.h"
#include "Void.h"

namespace Language
{

Expression* Pair::evalModifyEnv(Environment*& env)
{
    auto ret = l->evalModifyEnv(env);
    auto null = dynamic_cast<Void*>(r);

    // Returns if right value is void
    if (null != nullptr)
        return ret;

    ret = r->evalModifyEnv(env);

    return ret;
}

std::string Pair::toString() const
{
    return "(" + l->toString() + ":" + r->toString() + ")";
}


Expression* PairConstructorWithValue::apply(Expression* e, Environment* env)
{
    return new Pair(value, e->eval(env));
}

std::string PairConstructorWithValue::toString() const
{
    return "pair{" + value->toString() + "}";
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
