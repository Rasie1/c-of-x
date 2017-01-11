#include "Expressions/Function.h"
#include "System/Environment.h"
#include "Expressions/Void.h"
#include "Expressions/Equality.h"
#include "Expressions/Any.h"
#include "Expressions/Identifier.h"
#include "Expressions/String.h"
#include "Expressions/Operation.h"
#include "Expressions/Intersection.h"
#include "Expressions/ValueInSet.h"
#include "Expressions/Union.h"
#include "Expressions/Not.h"

ExpPtr Function::intersect(ExpPtrArg other, const Environment& env)
{
    auto value = Identifier::unwrapIfId(other, env);
    env.debugPrint("Intersecting function:" + this->show() + "\n");
    env.debugPrint("WITH: " + other->show() + "\n");




    // For some reason, here intersection of function and argument gives us the result.
    // I'm not really sure if it is correct, but now it fits everything
    if (checkType<Equals>(value))
    {
        auto eq = s_cast<Equals>(value);
        auto exactValue = eq->value;
        auto envCopy = env;
        auto applied = apply(exactValue, envCopy);
        if (checkType<Void>(applied))
            return applied;
        return make_ptr<Equals>(applied);
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

    if (checkType<Function>(value))
    {
        return make_ptr<Operation>(make_ptr<Union>(),
                                   shared_from_this(),
                                   other);
        // todo: not sure if it even works for non-closures
        // auto intersectionOfCodomains =
        //     make_ptr<Operation>(make_ptr<Intersection>(),
        //                         make_ptr<ValueInSet>(this->codomain()),
        //                         make_ptr<ValueInSet>(other->codomain()));
        // auto intersectionOfDomains =
        //     make_ptr<Operation>(make_ptr<Intersection>(),
        //                         make_ptr<ValueInSet>(this->domain()),
        //                         make_ptr<ValueInSet>(other->domain()));

        // auto leftDomain = 
        //     make_ptr<Operation>(make_ptr<Intersection>(),
        //                         make_ptr<Complement>(intersectionOfDomains),
        //                         make_ptr<ValueInSet>(this->domain());
        // auto rightDomain = 
        //     make_ptr<Operation>(make_ptr<Intersection>(),
        //                         make_ptr<Complement>(intersectionOfDomains),
        //                         make_ptr<ValueInSet>(other->domain());

        // auto intersectionClosure;
        // auto leftClosure;
        // auto rightClosure;
        // // todo: here could be something about fourth case

        // auto intersectionOfFunctions =
        //     make_ptr<Operation>(make_ptr<Union>(),
        //                         intersectionClosure,
        //                         make_ptr<Operation>(make_ptr<Union>(),
        //                                             leftClosure,
        //                                             rightClosure));
    }

    // Otherwise, leave it inevaluated
    return make_ptr<Operation>(make_ptr<Intersection>(),
                               shared_from_this(),
                               other);
}

bool ReversibleFunction::unapplyVariables(ExpPtrArg e, ExpPtrArg arg, Environment& env) const
{
    auto reversed = reverse()->apply(arg, env);

    auto ret = reversed->unapplyVariables(e, env);

    return ret;
}

ExpPtr Function::codomain()
{
    return make_ptr<Any>();
}

ExpPtr Function::domain()
{
    return make_ptr<Any>();
}
