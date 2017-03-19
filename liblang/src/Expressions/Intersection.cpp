#include "Expressions/Intersection.h"
#include "Expressions/Operation.h"
#include "Expressions/Identifier.h"
#include "Expressions/Void.h"
#include "System/Environment.h"
#include "System/Cast.h"

Intersection::Intersection()
    : Operator(false, 4, true)
{
}

ExpPtr Intersection::operate(ExpPtrArg first,
                             ExpPtrArg second,
                             Environment& env)
{
    if (auto idL = d_cast<Identifier>(first))
    if (auto idR = d_cast<Identifier>(second))
    if (idL->name == idR->name)
        return first;

    auto l = Identifier::unwrapIfId(first, env)->eval(env);
    auto r = Identifier::unwrapIfId(second, env)->eval(env);


    auto lr = l->intersect(r, env);

    // bool lf = false;
    // bool rf = false;
    // if (checkType<Operation>(lr))
    // {
    //     auto op = s_cast<Operation>(lr)->op;
    //     if (checkType<Intersection>(op))
    //         lf = true;
    // }
    // if (!lf)
    //     return lr;

     // auto rl = r->intersect(l, env);


     // auto operation = make_ptr<Operation>(make_ptr<Intersection>(), lr, rl);
//     auto result = operation->eval(env);

     // return operation;

    // if (checkType<Operation>(rl))
    // {
    //     auto op = s_cast<Operation>(rl)->op;
    //     if (checkType<Intersection>(op))
    //         rf = true;
    // }
    // if (!rf)
    //     return rl;

    // TODO: make this default behaviour and remove returns for both
    // branches (lines 40-41 and 51-52)
    // if (!lf || !rf)
    //     return make_ptr<Void>();

    return lr;
}

std::string Intersection::show() const
{
    return Intersection::defaultName;
}

const std::string Intersection::defaultName = "&";

bool Intersection::unapplyVariables(ExpPtrArg e,
                                    ExpPtrArg l,
                                    ExpPtrArg r,
                                    Environment &env)
{
    auto lUnapplied = l->unapplyVariables(e, env);
    auto rUnapplied = r->unapplyVariables(e, env);

    auto result = lUnapplied && rUnapplied;

    return result;
}

ExpPtr Intersection::apply(ExpPtrArg l, ExpPtrArg r, ExpPtrArg e, Environment &env)
{
    auto envc = env;
    auto lApplied = l->apply(e, env);
    auto rApplied = r->apply(e, env);

    auto operation = make_ptr<Operation>(make_ptr<Intersection>(), lApplied, rApplied);
    auto result = operation->eval(env);

    return result;
}

ExpPtr Intersection::intersect(ExpPtrArg l, ExpPtrArg r, ExpPtrArg e, Environment &env)
{
    auto envc = env;
    auto lApplied = l->intersect(e, env);
    auto rApplied = r->intersect(e, env);

    auto operation = make_ptr<Operation>(make_ptr<Intersection>(), lApplied, rApplied);
    auto result = operation->eval(env);

    return result;
}
