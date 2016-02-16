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

std::string Expression::show() const
{
    return "expr";
}

bool Expression::hasNonOpVariable(Environment* env) const
{
    return false;
}

bool Expression::unwind(ExpPtr& lvalue,
                        ExpPtr& rvalue,
                        Environment*& env)
{
    return false;
}

ExpPtr Expression::intersect(ExpPtrArg& other, Environment* env)
{
    return make_ptr<Void>();
}
