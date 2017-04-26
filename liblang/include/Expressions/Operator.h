#pragma once
#include "Expressions/Morphism.h"

class Operator : public Morphism
{
public:
    Operator(bool isRightAssociative = false, int priority = 5, bool splitting = false);

    virtual Object operate(const Object& first,
                           const Object& second,
                           Environment& env) = 0;

    bool isRightAssociative;
    int priority;
    const std::string name;
    bool splitting;

    bool operator==(const Expression& other) const override;

    virtual Object apply(const Object& l, const Object& r, const Object& e, Environment &env);
    virtual Object intersect(const Object& l, const Object& r, const Object& e, Environment &env);
    virtual Object element(const Object& l, const Object& r, Environment &env);
    virtual bool unapplyVariables(const Object& e, const Object& l, const Object& r, Environment &env);

    virtual Object partialApplyLeft  (const Object& e, Environment& env);
    virtual Object partialApplyRight (const Object& e, Environment& env);
    virtual Object partialApplyNoArgs(Environment& env);

    Object apply(const Object& e, Environment& env) override;

    virtual std::string show() const;
private:
};

