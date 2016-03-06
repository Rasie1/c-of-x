#include "Subtraction.h"
#include <string>
#include "Integer.h"
#include "Operation.h"
#include "Addition.h"
#include "Identifier.h"
#include "TypeError.h"
#include "Environment.h"

Subtraction::Subtraction()
    : CalculationOperator(false, 5)
{
}

ExpPtr Subtraction::calculate(ExpPtrArg l, ExpPtrArg r) const
{
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
