#include "Assignment.h"
#include "Variable.h"
#include "Environment.h"
#include "Pattern.h"
#include <string>
#include "Operation.h"

namespace Language
{

Expression* Assignment::operate(Expression* first,
                                Expression* second,
                                Environment*& env)
{
    env = env->add(first->pattern(),
                   second);
    return second;
}

std::string Assignment::toString() const
{
    return Assignment::defaultName;
}

const std::string Assignment::defaultName = "=";


}
