#include "Subtraction.h"
#include <string>
#include "Integer.h"
#include "Operation.h"
#include "Addition.h"
#include "Identifier.h"
#include "TypeError.h"

Subtraction::Subtraction()
    : CalculationOperator(false, 5)
{
}

ExpPtr Subtraction::operate(ExpPtrArg first,
                            ExpPtrArg second,
                            Environment*& env) const
{
    auto l = first->eval(env);
    auto r = second->eval(env);
    auto firstInteger  = d_cast<Integer>(l);
    auto secondInteger = d_cast<Integer>(r);

    if (!firstInteger || !secondInteger)
    {
        auto operation = make_ptr<Operation>(make_ptr<Subtraction>(), l, r);
        return make_ptr<TypeError>(operation,
                                   make_ptr<Identifier>("int"),
                                   make_ptr<Identifier>("?"));
    }

    return make_ptr<Integer>(firstInteger->value - secondInteger->value);
}

std::string Subtraction::show() const
{
    return Subtraction::defaultName;
}

const std::string Subtraction::defaultName = "-";


bool Subtraction::unwind(ExpPtr& left,
                         ExpPtr& right,
                         ExpPtr& lvalue,
                         ExpPtr& rvalue,
                         Environment*& env)
{
    auto newEnv = env;
    if (left->hasNonOpVariable(env))
    {
        if (right->hasNonOpVariable(env))
            return false;
        lvalue = left;
        rvalue = make_ptr<Operation>(
                    make_ptr<Addition>(),
                    rvalue,
                    right);
        return true;
    }
    else if (right->hasNonOpVariable(env))
    {
        lvalue = right;
        rvalue = make_ptr<Operation>(
                    make_ptr<Subtraction>(),
                    left,
                    rvalue);
        return true;
    }
    return false;
}
