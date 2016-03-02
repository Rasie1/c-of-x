#include "Expression.h"
#include <stdexcept>
#include "TypeError.h"
#include "Application.h"
#include "Operation.h"
#include "Environment.h"
#include "Intersection.h"
#include "Void.h"

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
//    std::cout << show() << "::::" << std::hash<std::string>()(show()) << std::endl;
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

bool Expression::unwind(ExpPtr& lvalue,
                        ExpPtr& rvalue,
                        Environment& env)
{
    return false;
}

ExpPtr Expression::intersect(ExpPtrArg& other, const Environment& env)
{
    return make_ptr<Operation>(make_ptr<Intersection>(), shared_from_this(), other);
}

void Expression::unapplyVariables(ExpPtrArg e, Environment& env) const
{

}
