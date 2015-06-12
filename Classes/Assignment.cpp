#include "Assignment.h"
#include "Variable.h"
#include "Environment.h"
#include "Pattern.h"

namespace Language
{

Assignment::Assignment()
{

}

Expression* Assignment::apply(Expression* e, Environment* env)
{
    return new AssignmentOfValue(e);
}

std::string Assignment::toString() const
{
    return "=";
}


AssignmentOfValue::AssignmentOfValue(Expression* e)
    : value(e)
{

}

AssignmentOfValue::~AssignmentOfValue()
{
    delete value;
}

Expression* AssignmentOfValue::apply(Expression* e, Environment* env)
{
    return value;
}

std::string AssignmentOfValue::toString() const
{
    return "(= " + value->toString() + ")";
}

}
