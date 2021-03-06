#include "Expressions/Union.h"
#include "Expressions/Operation.h"
#include "Expressions/Void.h"
#include "Expressions/Identifier.h"
#include "Expressions/Intersection.h"
#include "System/Cast.h"
#include <algorithm>
#include <list>

Union::Union()
    : Operator(false, 4, true)
{
}

//bool findSameOperand(const Object& where, const Object& what)
//{
//    if (*where == *what)
//        return true;
//    return false;

////    auto op = cast<Operation>(env, where);
////    if (op && cast<Union>(env, op->op))
////        return findSameOperand(op->left, what) ||
////               findSameOperand(op->right, what);
////    return false;
//}


//bool isVoid(Object x)
//{
//    if (cast<Void>(env, x))
//        return true;

//    auto id = cast<Identifier>(env, x);

//    if (id && id->name == Void::defaultName)
//        return true;

//    return false;
//}

//std::list<Object> unroll(const Object& x)
//{
//    if (isVoid(x))
//        return {};
//    auto op = cast<Operation>(env, x);
//    if (op != nullptr)
//    {
//        if (checkType<Union>(env, op->op))
//        {
//            std::list<Object> ret;
//            ret.splice(ret.begin(), unroll(op->left));
//            ret.splice(ret.begin(), unroll(op->right));
//            return ret;
//        }
//        else
//            return {x};
//    }
//    else
//        return {x};
//}

//Object makeTree(const std::list<Object>::iterator& p0,
//                const std::list<Object>::iterator& p1)
//{
//    if (std::distance(p0, p1) == 0)
//        return *p0;
//    if (std::distance(p0, p1) == 1)
//        return makeObject<Operation>(makeObject<Union>(), *p0, *p1);
//    auto center = std::next(p0, std::distance(p0, p1) / 2);
//    return makeObject<Operation>(makeObject<Union>(),
//                               makeTree(p0, center),
//                               makeTree(std::next(center), p1));
//}

//Object Union::make_old(const std::vector<Object>& exps)
//{
//    // TODO: make generic tree algorithm

//    std::list<Object> xs;
//    for (auto &x : exps)
//        xs.splice(xs.begin(), unroll(x));

//    if (xs.size() == 0)
//        return makeObject<Void>();


//    for (auto current = xs.begin(); current != xs.end(); ++current)
//    for (auto tested  = std::next(current); tested != xs.end(); ++tested)
//    {
//        if (**current == **tested)
//        {
//            --tested;
//            xs.erase(std::next(tested));
//        }
//    }

//    return makeTree(xs.begin(), std::prev(xs.end()));

//    if (xs.size() == 1)
//        return xs.front();

//    if (xs.size() == 2)
//    {
//        return makeObject<Operation>(makeObject<Union>(),
//                                   *xs.begin(),
//                                   *std::next(xs.begin()));
//    }

//    if (xs.size() == 3)
//    {
//        return makeObject<Operation>(makeObject<Union>(), *xs.begin(),
//                                   makeObject<Operation>(makeObject<Union>(),
//                                                       *std::next(xs.begin(), 1),
//                                                       *std::next(xs.begin(), 2)));
//    }

//    if (xs.size() == 4)
//    {
//        return makeObject<Operation>(makeObject<Union>(),
//                                   makeObject<Operation>(makeObject<Union>(),
//                                                       *std::next(xs.begin(), 0),
//                                                       *std::next(xs.begin(), 1)),
//                                   makeObject<Operation>(makeObject<Union>(),
//                                                       *std::next(xs.begin(), 2),
//                                                       *std::next(xs.begin(), 3)));
//    }

//    return makeObject<Void>();

//}

Object Union::operate(const Object& first,
                      const Object& second,
                      Environment& env)
{
    if (auto idL = cast<Identifier>(env, first))
    if (auto idR = cast<Identifier>(env, second))
    if (idL->name == idR->name)
        return first;

    auto l = Identifier::unwrapIfId(first, env);
    auto r = Identifier::unwrapIfId(second, env);

    auto lr = l->unionize(r, env);

    return lr;

    bool lf = false;
    bool rf = false;
    if (auto op = cast<Operation>(env, lr))
    {
        if (checkType<Union>(env, op))
            lf = true;
    }
    if (!lf)
        return lr;

    auto rl = r->unionize(l, env);

    if (auto op = cast<Operation>(env, rl))
    {
        if (checkType<Union>(env, op))
            rf = true;
    }
    if (!rf)
        return rl;

    return lr;
//    auto l = first;
//    auto r = second;


//    auto x = {l, r};
//    return make(std::begin(x), std::end(x));
}

std::string Union::show() const
{
    return Union::defaultName;
}

const std::string Union::defaultName = "|";

bool Union::unapplyVariables(const Object& e,
                             const Object& l,
                             const Object& r,
                             Environment &env)
{
    // TODO: when adding to env, we should union branches and not intersect
    // this is not accessible right now and most likely I'll have to add
    // flag to every unapply call

    // Or, better way could be returning new env value somehow

    auto lUnapplied = l->unapplyVariables(e, env);
    auto rUnapplied = r->unapplyVariables(e, env);

    auto result = lUnapplied || rUnapplied;

    return result;
}

Object Union::intersect(const Object& l, const Object& r, const Object& e, Environment &envc)
{
    auto env = envc;
    // TODO: preserve identifier?
    auto operation = makeOperation<Intersection>(l, e);
    auto leftIntersection = operation;

    if (checkType<Void>(env, leftIntersection))
    {
        auto operation = makeOperation<Intersection>(r, e);
        auto rightIntersection = operation;

        return rightIntersection;
    }

    return leftIntersection;
}

Object Union::make(const std::vector<Object>::iterator& begin, 
                   const std::vector<Object>::iterator& end,
                   Environment& env)
{
    auto next = std::next(begin);
    if (next == end)
        return *begin;
    if (checkType<Void>(env, *begin))
    {
        return make(next, end, env);
    }
    return makeOperation<Union>(*begin, make(next, end, env));
}
