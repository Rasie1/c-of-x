#include "Variable.h"
#include "Environment.h"
#include "UnknownName.h"
#include "PatternVariable.h"
#include <string>
#include <list>

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
    return make_ptr<PatternVariable>(name);
}

bool Variable::match(ExpPtrArg other) const
{
    auto variable = d_cast<Variable>(other);
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

std::string Variable::show() const
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

void Variable::getAllVariables(
        std::vector<std::shared_ptr<Variable>>& variables)
{
    variables.push_back(
                s_cast<Variable>(shared_from_this()));
}
