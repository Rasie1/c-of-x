#include "Closure.h"
#include <string>
#include "Function.h"
#include "Environment.h"
#include "Error.h"
#include "Identifier.h"
#include "Void.h"

Closure::Closure(ExpPtrArg argument,
                 ExpPtrArg body,
                 Environment* env,
                 int envSize)
    : body(body),
      argument(argument),
      env(env),
      envSize(envSize)
{

}

Closure::~Closure()
{
    // delete envSize elements
}

ExpPtr Closure::apply(ExpPtrArg e, Environment*& env) const
{
//    ExpPtr lvalue, rvalue;

//    if (d_cast<Identifier>(body))
//        rvalue = env->get(body);
//    if (rvalue == nullptr)
//        rvalue = body;

//    if (d_cast<Identifier>(lvalue))
//        env = env->add(lvalue, rvalue);
//    else
//        while (lvalue->unwind(lvalue, rvalue, env));





    ExpPtr lvalue = argument;
    ExpPtr rvalue = body;
    while (lvalue->unwind(lvalue, rvalue, env));

    auto variable = d_cast<Identifier>(lvalue);
    auto value = d_cast<DataType>(lvalue);

    if (variable)
    {
        auto newEnv = this->env->add(variable, e);
        auto evaluated = rvalue->eval(newEnv);
        if (d_cast<Identifier>(evaluated))
            evaluated = newEnv->get(evaluated);
        return evaluated;
    }
    else if (value)
    {
        if (value->match(e))
        {
            auto newEnv = env;
            return rvalue->eval(newEnv);
        }
        else
        {
            return make_ptr<Void>();
        }
    }
    else
        return make_ptr<ErrorWithMessage>("Incorrect argument definition");
}

std::string Closure::show() const
{
//    std::string ret = "[";
//    auto top = env;
//    for (int i = 0; i < envSize; ++i)
//    {
//        ret +=top->top().first->show()
//            + std::string(" = ")
//            + top->top().second->show()
//            + std::string(";");
//        top = top->getNext();
//    }
//    ret += std::string("]");
    return "[|" + argument->show() + " => " + body->show() + "|]";
}