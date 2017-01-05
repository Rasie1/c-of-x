#include "Expressions/Union.h"
#include "Expressions/Operation.h"
#include "Expressions/Void.h"
#include "Expressions/Identifier.h"
#include <algorithm>
#include <list>

Union::Union()
    : Operator(false, 4, true)
{
}

//bool findSameOperand(ExpPtrArg where, ExpPtrArg what)
//{
//    if (*where == *what)
//        return true;
//    return false;

////    auto op = d_cast<Operation>(where);
////    if (op && d_cast<Union>(op->op))
////        return findSameOperand(op->left, what) ||
////               findSameOperand(op->right, what);
////    return false;
//}


//bool isVoid(ExpPtr x)
//{
//    if (d_cast<Void>(x))
//        return true;

//    auto id = d_cast<Identifier>(x);

//    if (id && id->name == Void::defaultName)
//        return true;

//    return false;
//}

//std::list<ExpPtr> unroll(ExpPtrArg x)
//{
//    if (isVoid(x))
//        return {};
//    auto op = d_cast<Operation>(x);
//    if (op != nullptr)
//    {
//        if (checkType<Union>(op->op))
//        {
//            std::list<ExpPtr> ret;
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

//ExpPtr makeTree(const std::list<ExpPtr>::iterator& p0,
//                const std::list<ExpPtr>::iterator& p1)
//{
//    if (std::distance(p0, p1) == 0)
//        return *p0;
//    if (std::distance(p0, p1) == 1)
//        return make_ptr<Operation>(make_ptr<Union>(), *p0, *p1);
//    auto center = std::next(p0, std::distance(p0, p1) / 2);
//    return make_ptr<Operation>(make_ptr<Union>(),
//                               makeTree(p0, center),
//                               makeTree(std::next(center), p1));
//}

//ExpPtr Union::make_old(const std::vector<ExpPtr>& exps)
//{
//    // TODO: make generic tree algorithm

//    std::list<ExpPtr> xs;
//    for (auto &x : exps)
//        xs.splice(xs.begin(), unroll(x));

//    if (xs.size() == 0)
//        return make_ptr<Void>();


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
//        return make_ptr<Operation>(make_ptr<Union>(),
//                                   *xs.begin(),
//                                   *std::next(xs.begin()));
//    }

//    if (xs.size() == 3)
//    {
//        return make_ptr<Operation>(make_ptr<Union>(), *xs.begin(),
//                                   make_ptr<Operation>(make_ptr<Union>(),
//                                                       *std::next(xs.begin(), 1),
//                                                       *std::next(xs.begin(), 2)));
//    }

//    if (xs.size() == 4)
//    {
//        return make_ptr<Operation>(make_ptr<Union>(),
//                                   make_ptr<Operation>(make_ptr<Union>(),
//                                                       *std::next(xs.begin(), 0),
//                                                       *std::next(xs.begin(), 1)),
//                                   make_ptr<Operation>(make_ptr<Union>(),
//                                                       *std::next(xs.begin(), 2),
//                                                       *std::next(xs.begin(), 3)));
//    }

//    return make_ptr<Void>();

//}

ExpPtr Union::operate(ExpPtrArg first,
                      ExpPtrArg second,
                      Environment& env) const
{
    auto l = first->eval(env);
    auto r = second->eval(env);

    auto lr = l->unionize(r, env);

    bool lf = false;
    bool rf = false;
    if (checkType<Operation>(lr))
    {
        auto op = s_cast<Operation>(lr)->op;
        if (checkType<Union>(op))
            lf = true;
    }
    if (!lf)
        return lr;

    auto rl = r->unionize(l, env);

    if (checkType<Operation>(rl))
    {
        auto op = s_cast<Operation>(rl)->op;
        if (checkType<Union>(op))
            rf = true;
    }
    if (!rf)
        return rl;

    return lr;
//    auto l = first->eval(env);
//    auto r = second->eval(env);


//    auto x = {l, r};
//    return make(std::begin(x), std::end(x));
}

std::string Union::show() const
{
    return Union::defaultName;
}

const std::string Union::defaultName = "|";
