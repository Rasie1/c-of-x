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

Object Intersection::operate(const Object& first,
                             const Object& second,
                             Environment& env)
{
    if (auto idL = cast<Identifier>(env, first))
    if (auto idR = cast<Identifier>(env, second))
    if (idL->name == idR->name)
        return first;

    auto l = Identifier::unwrapIfId(first, env);
    auto r = Identifier::unwrapIfId(second, env);


    auto lr = l->intersect(r, env);

    // temporary
    return lr;

    // bool lf = false;
    // bool rf = false;
    // if (checkType<Operation>(env, lr))
    // {
    //     auto op = s_cast<Operation>(env, lr)->op;
    //     if (checkType<Intersection>(env, op))
    //         lf = true;
    // }
    // if (!lf)
    //     return lr;

    auto rl = r->intersect(l, env);

//    if (checkType<Void>(env, lr))
//        return rl;
//    if (checkType<Void>(env, rl))
//        return lr;

    auto operation = makeOperation<Intersection>(lr, rl);
    auto result = operation;

    return result;

    // if (checkType<Operation>(env, rl))
    // {
    //     auto op = s_cast<Operation>(env, rl)->op;
    //     if (checkType<Intersection>(env, op))
    //         rf = true;
    // }
    // if (!rf)
    //     return rl;

    // TODO: make this default behaviour and remove returns for both
    // branches (lines 40-41 and 51-52)
    // if (!lf || !rf)
    //     return makeObject<Void>();

    // return lr;
}

std::string Intersection::show() const
{
    return Intersection::defaultName;
}

const std::string Intersection::defaultName = "&";

bool Intersection::unapplyVariables(const Object& e,
                                    const Object& l,
                                    const Object& r,
                                    Environment &env)
{
    auto lUnapplied = l->unapplyVariables(e, env);
    auto rUnapplied = r->unapplyVariables(e, env);

    auto result = lUnapplied && rUnapplied;

    return result;
}

Object Intersection::apply(const Object& l, const Object& r, const Object& e, Environment &env)
{
    auto envc = env;
    auto lApplied = l->apply(e, env);
    auto rApplied = r->apply(e, env);

    auto operation = makeOperation<Intersection>(lApplied, rApplied);
    auto result = operation;

    return result;
}

Object Intersection::intersect(const Object& l, const Object& r, const Object& e, Environment &env)
{
    auto envc = env;
    auto lApplied = l->intersect(e, env);
    auto rApplied = r->intersect(e, env);

    auto operation = makeOperation<Intersection>(lApplied, rApplied);
    auto result = operation;

    return result;
}

Object Intersection::element(const Object& l, const Object& r, Environment &env)
{
    auto envc = env;
    auto rElement = r->element(env);
    auto lElement = l->element(env);

    auto operation = makeOperation<Intersection>(lElement, rElement);
    auto result = operation;

    return result;
}
