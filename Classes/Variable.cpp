#include "Variable.h"
#include "Environment.h"
#include "UnknownName.h"
#include <string>
#include <list>
#include "Error.h"

ExpPtr Variable::eval(Environment*& env) const
{
    auto ret = env->get(std::const_pointer_cast<Expression>(shared_from_this()));
    if (ret == nullptr)
        return make_ptr<ErrorWithMessage>("Unknown name \"" + name + "\"");
        // return std::const_pointer_cast<Expression>(shared_from_this());

    return ret->eval(env);
}

bool Variable::match(ExpPtrArg other) const
{
    auto variable = d_cast<Variable>(other);
    if (variable)
        return name == variable->name;

    return false;
}

std::string Variable::show() const
{
    return name;
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
