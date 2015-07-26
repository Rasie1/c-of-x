#include "Function.h"
#include <iostream>
#include "Pattern.h"
#include "Closure.h"
#include "Environment.h"
#include "String.h"
#include "Variable.h"
#include <string>
#include <iostream>

namespace Language
{

Function::Function(const ExpPtr& body,
                   const ExpPtr& argument)
    : body(body),
      argument(argument)
{
}

Function::~Function()
{
}

ExpPtr Function::apply(const ExpPtr& e,
                       Environment*& env) const
{
    /*auto newEnv1 = env;
    auto newEnv2 = env->add(argument,
                            e->eval(newEnv1));*/
    //if (!argument->match(e, env))
    //    return std::make_shared<String>("!No Match!");
    std::vector<std::shared_ptr<Variable>> argumentVariables;
    argument->getAllVariables(argumentVariables);
    auto newEnv1 = env;
    for (auto x : argumentVariables)
    {
        std::cout << x->toString() << std::endl;
        newEnv1 = newEnv1->add(x, e);
    }
//    newEnv1->add(std::const_pointer_cast<Expression>(shared_from_this()),
//                 std::make_shared<PatternVariable>(this->name));
    return body->eval(newEnv1);
}

ExpPtr Function::evaluation(Environment*& env) const
{
    throw  std::logic_error("we aren\'t supposed to get here");
    return std::make_shared<Closure>(
                std::static_pointer_cast<Function>(
                    std::const_pointer_cast<Expression>(shared_from_this())),
                env);
}

std::string Function::toString() const
{
    return "(" + argument->toString() + "){" + body->toString() + "}";
}

}
