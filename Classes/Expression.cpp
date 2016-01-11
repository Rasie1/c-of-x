#include "Expression.h"
#include <stdexcept>
#include "TypeError.h"
#include "Application.h"
#include "Operation.h"
#include "Environment.h"

Expression::Expression()
{

}

bool Expression::match(ExpPtrArg other) const
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
