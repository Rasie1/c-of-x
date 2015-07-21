#include "Variable.h"
#include "Environment.h"
#include "UnknownName.h"
#include "PatternVariable.h"
#include <string>

namespace Language
{

ExpPtr Variable::evaluation(Environment*& env) const
{
    auto ret = env->get(std::const_pointer_cast<Expression>(shared_from_this()));
    if (ret == nullptr)
        return std::const_pointer_cast<Expression>(shared_from_this());

    auto newEnv = env;
    return ret;//ret->eval(newEnv);
}

PatPtr Variable::pattern() const
{
    return std::make_shared<PatternVariable>(name);
}

bool Variable::match(const ExpPtr& other,
                     Environment* env) const
{
    auto variable = std::dynamic_pointer_cast<Variable>(other);
    if (variable)
        return name == variable->name;

    return false;
}

bool Variable::isOperator(Environment* env) const
{
    auto e = env->get(std::const_pointer_cast<Expression>(shared_from_this())) ;
    if (e)
        return e->isOperator(env);
    else
        return false;
}

std::string Variable::toString() const
{
    return "$" + name;
}

bool Variable::unwind(ExpPtr& lvalue,
                      ExpPtr& rvalue,
                      Environment*& env)
{
    auto evaluated = env->get(rvalue);
    if (evaluated == nullptr)
        return false;
    rvalue = evaluated;

    return true;
}

bool Variable::hasNonOpVariable(Environment* env) const
{
    return env->get(std::const_pointer_cast<Expression>(shared_from_this())) == nullptr;
}

}
