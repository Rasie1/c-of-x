#include "Mutation.h"
#include "Variable.h"
#include "Environment.h"
#include "Pattern.h"

namespace Language
{

Mutation::Mutation()
{

}

Expression* Mutation::apply(Expression* e, Environment* env)
{
    return new MutationWithValue(e);
}


std::string Mutation::toString() const
{
    return defaultName;
}

const std::string Mutation::defaultName = "mutate";



MutationWithValue::MutationWithValue(Expression* e)
    : value(e)
{

}

MutationWithValue::~MutationWithValue()
{
    delete value;
}

Expression* MutationWithValue::apply(Expression* e, Environment* env)
{
    return value;
}

std::string MutationWithValue::toString() const
{
    return "(:= " + value->toString() + ")";
}

}
