#pragma once
#include "Expressions/Morphism.h"
#include "System/Cast.h"

class Operator;
class Operation : public Morphism
{
public:
    Operation(const std::shared_ptr<Operator>& op,
              const Object& left,
              const Object& right);

    Object eval(Environment& env) override;
    std::string show() const override;

    Object apply(const Object& other, Environment& env) override;
    Object intersect(const Object& other, Environment& env) override;
    Object element(const Environment& env) override;
    bool hasFreeVariables(const Environment& env) const;
    bool unapplyVariables(const Object& e, Environment& env) override;

    bool operator==(const Expression& other) const override;

    std::shared_ptr<Operator> op;
    Object left;
    Object right;
};

template <class T>
Object makeOperation(const Object& l, const Object& r)
{
    return makeObject<Operation>(std::make_shared<T>(),
                                 l, r);
}

#include "Expressions/Intersection.h"

inline bool findInIntersection(const Object& l, const Object& r)
{
    if (*l.expression == *r.expression)
        return true;

    if (auto lop = castNoEval<Operation>(l))
    if (checkTypeNoEval<Intersection>(lop->op))
        return findInIntersection(lop->left,  r) 
            || findInIntersection(lop->right, r);
}

template <>
inline Object makeOperation<Intersection>(const Object& l, const Object& r)
{
    if (findInIntersection(l, r))
        return l;
    if (findInIntersection(r, l))
        return r;

    return makeObject<Operation>(std::make_shared<Intersection>(),
                                 l, r);
}


