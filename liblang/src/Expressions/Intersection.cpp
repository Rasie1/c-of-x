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
    if (auto idL = d_cast<Identifier>(first))
    if (auto idR = d_cast<Identifier>(second))
    if (idL->name == idR->name)
        return first;

    auto l = Identifier::unwrapIfId(first, env)->eval(env);
    auto r = Identifier::unwrapIfId(second, env)->eval(env);


    auto lr = l->intersect(r, env);

    // temporary
    return lr;

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

    auto rl = r->intersect(l, env);

//    if (checkType<Void>(lr))
//        return rl;
//    if (checkType<Void>(rl))
//        return lr;

    auto operation = makeObject<Operation>(makeObject<Intersection>(), lr, rl);
    auto result = operation->eval(env);

    return result;

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

    auto operation = makeObject<Operation>(makeObject<Intersection>(), lApplied, rApplied);
    auto result = operation->eval(env);

    return result;
}

Object Intersection::intersect(const Object& l, const Object& r, const Object& e, Environment &env)
{
    auto envc = env;
    auto lApplied = l->intersect(e, env);
    auto rApplied = r->intersect(e, env);

    auto operation = makeObject<Operation>(makeObject<Intersection>(), lApplied, rApplied);
    auto result = operation->eval(env);

    return result;
}
