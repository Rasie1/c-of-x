#include "Expressions/Morphism.h"
#include "System/Environment.h"
#include "Expressions/Void.h"
#include "Expressions/Equality.h"
#include "Expressions/Any.h"
#include "Expressions/Identifier.h"
#include "Expressions/String.h"
#include "Expressions/Closure.h"
#include "Expressions/Operation.h"
#include "Expressions/Intersection.h"
#include "Expressions/ValueInSet.h"
#include "Expressions/Union.h"
#include "Expressions/Not.h"
#include "System/Cast.h"

Object Morphism::intersect(const Object& other, Environment& envc)
{
    auto env = envc;
    auto value = Identifier::unwrapIfId(other, env);
    env.debugPrint("Intersecting function: " + this->show() + "\n");
    env.debugPrint("WITH: " + other->show() + "\n");



    if (this->equals(other, env))
    {
        return thisObject();
    }


    // For some reason, here intersection of function and argument gives us the result.
    // I'm not really sure if it is correct, but now it fits everything
    if (auto eq = cast<Equals>(env, value))
    {
        auto exactValue = eq->value;
        auto envCopy = env;
        auto applied = apply(exactValue, envCopy);
        if (checkType<Void>(env, applied))
            return applied;
        return makeObject<Equals>(applied);
    }

    // Same thing; possibly there are incorrect cases. Needs some research. What I've got is:
    // Image of intersection should be equal intersection of images of input functions
    // Remains should be equal images of corresponding functions
    // For example,
    //    1..5 -> 1..5
    //    3..7 -> 3..7
    // is the same as
    //    1 -> 1..5
    //    2 -> 1..5
    //    3 -> 3..5
    //    4 -> 3..5
    //    5 -> 3..5
    //    6 -> 3..7
    //    7 -> 3..7

    auto newEnv = env;
    auto evaluated = value->eval(newEnv);


    // if (auto other = cast<Morphism>(env, evaluated))
    // {
    //     // return makeOperation<Union>(thisObject(),
    //     //                             other);
    //     // todo: not sure if it even works for non-closures
    //     auto intersectionOfCodomains =
    //         makeOperation<Intersection>(this->codomain(),
    //                             other->codomain());
    //     auto intersectionOfDomains =
    //         makeOperation<Intersection>(this->domain(),
    //                             other->domain());

    //     auto leftDomain = 
    //         makeOperation<Intersection>(makeObject<Complement>(intersectionOfDomains),
    //                             this->domain());
    //     auto rightDomain = 
    //         makeOperation<Intersection>(makeObject<Complement>(intersectionOfDomains),
    //                             other->domain());

    //     auto intersectionClosure = 
    //         makeObject<Closure>(makeObject<ValueInSet>(intersectionOfDomains),
    //                           makeObject<ValueInSet>(intersectionOfCodomains),
    //                           newEnv);
    //     auto leftClosure =
    //         makeObject<Closure>(makeObject<ValueInSet>(leftDomain),
    //                           makeObject<ValueInSet>(this->codomain()),
    //                           newEnv);
    //     auto rightClosure =
    //         makeObject<Closure>(makeObject<ValueInSet>(rightDomain),
    //                             makeObject<ValueInSet>(other->codomain()),
    //                             newEnv);
    //     // // todo: here could be something about fourth case

    //     auto intersectionOfFunctions =
    //         makeOperation<Union>(intersectionClosure,
    //                              makeOperation<Union>(leftClosure,
    //                                                   rightClosure));

    //     return intersectionOfFunctions;
    // }

    return makeOperation<Intersection>(thisObject(),
                                       evaluated);

}

bool Isomorphism::unapplyVariables(const Object& e, Environment& env)
{
    // TODO
//    return false;
    auto reversed = cast<Morphism>(env, inverse());
    //;->apply(makeObject<Any>(), env);

//   auto ret = reversed->unapplyVariables(e, env);
    auto ret = reversed->apply(e, env);

    return !checkType<Void>(env, ret);
}

Object Morphism::eval(Environment& env)
{
    return thisObject();
}

bool Morphism::unapplyVariables(const Object& e, Environment& env)
{
    if (checkType<Any>(env, e))
        return true;
    return *this == *e;
}
