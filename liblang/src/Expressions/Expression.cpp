#include "Expressions/Expression.h"
#include <stdexcept>
#include <iostream>
#include "Expressions/TypeError.h"
#include "Expressions/Application.h"
#include "Expressions/Operation.h"
#include "System/Environment.h"
#include "Expressions/Intersection.h"
#include "Expressions/ValueInSet.h"
#include "Expressions/Union.h"
#include "Expressions/Void.h"
#include "Expressions/Not.h"

using namespace std;

Expression::Expression()
{

}

bool Expression::operator==(const Expression& other) const
{
    return false;
}

bool Expression::operator< (const Expression& other) const
{
    return hash() < other.hash();
}

size_t Expression::hash() const
{
    return std::hash<std::string>()(show());
}

std::string Expression::show() const
{
    return "expr";
}

bool Expression::hasFreeVariables(const Environment& env) const
{
    return false;
}

Object Expression::intersect(const Object& other, const Environment& env)
{
    return make_ptr<Operation>(make_ptr<Intersection>(), shared_from_this(), other);
}

Object Expression::unionize(const Object& other, const Environment& env)
{
    return make_ptr<Operation>(make_ptr<Union>(), shared_from_this(), other);
}

Object Expression::complement(const Environment& env)
{
    return make_ptr<Complement>(shared_from_this());
}

Object Expression::element(const Environment& env)
{
    return make_ptr<ValueInSet>(shared_from_this());
}

Object Expression::apply(const Object& other, Environment& env)
{
    return make_ptr<Void>();
}

bool Expression::unapplyVariables(const Object& e, Environment& env)
{
    // TODO: we aren't supposed to get here
    return false;
}
