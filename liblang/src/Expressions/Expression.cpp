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

bool Expression::hasFreeVariables(const Environment& env)
{
    return false;
}

Object Expression::intersect(const Object& other, const Environment& env)
{
    return makeOperation<Intersection>(thisObject(), other);
}

Object Expression::unionize(const Object& other, const Environment& env)
{
    return makeOperation<Union>(thisObject(), other);
}

Object Expression::complement(const Environment& env)
{
    return makeObject<Complement>(thisObject());
}

Object Expression::element(const Environment& env)
{
    return makeObject<ValueInSet>(thisObject());
}

Object Expression::apply(const Object& other, Environment& env)
{
    return makeObject<Void>();
}

bool Expression::unapplyVariables(const Object& e, Environment& env)
{
    // TODO: we aren't supposed to get here
    return false;
}

Object Expression::thisObject()
{
    return Object(shared_from_this());
}
