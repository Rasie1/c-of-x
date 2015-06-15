#include "Assignment.h"
#include "Variable.h"
#include "Environment.h"
#include "Pattern.h"

namespace Language
{

Assignment::Assignment()
{

}

Expression* Assignment::applyConstEnv(Expression* e, Environment* env)
{
    return new AssignmentOfValue(e);
}

std::string Assignment::toString() const
{
    return defaultName;
}

const std::string Assignment::defaultName = "assign";


AssignmentOfValue::AssignmentOfValue(Expression* e)
    : value(e)
{
}

AssignmentOfValue::~AssignmentOfValue()
{
    delete value;
}

Expression* AssignmentOfValue::applyConstEnv(Expression* e, Environment* env)
{
    throw std::exception();
    return value;
}

Expression* AssignmentOfValue::apply(Expression* e, Environment*& env)
{
    env = env->add(e->pattern(),
                   value);
    return value;
}

std::string AssignmentOfValue::toString() const
{
    return "(= " + value->toString() + ")";
}

}
