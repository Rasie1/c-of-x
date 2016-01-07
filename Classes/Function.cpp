#include "Function.h"
#include <iostream>
#include "Pattern.h"
#include "Closure.h"
#include "Environment.h"
#include "String.h"
#include "Variable.h"
#include <string>
#include <iostream>


// ExpPtr Function::apply(ExpPtrArg e,
//                        Environment*& env) const
// {
//     /*auto newEnv1 = env;
//     auto newEnv2 = env->add(argument,
//                             e->eval(newEnv1));*/
// //    auto evaluated = argument->eval(env);
// //    if (!evaluated->pattern()->match(e))
// //        return make_ptr<String>("!No Match!");

//     auto variable = d_cast<Variable>(argument);
//     auto newEnv1 = env;
//     newEnv1 = newEnv1->add(variable, e);
// //    newEnv1->add(std::const_pointer_cast<Expression>(shared_from_this()),
// //                 make_ptr<PatternVariable>(this->name));
//     return body->eval(newEnv1);
// }

// ExpPtr Function::evaluation(Environment*& env) const
// {
//     throw  std::logic_error("we aren\'t supposed to get here");
//     return make_ptr<Closure>(
//                 s_cast<Function>(
//                     std::const_pointer_cast<Expression>(shared_from_this())),
//                 env);
// }

// std::string Function::show() const
// {
//     return "(" + argument->show() + "){" + body->show() + "}";
// }
