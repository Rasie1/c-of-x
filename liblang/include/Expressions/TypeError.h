#pragma once
#include "Expressions/Error.h"
#include <string>

class TypeError : public Error
{
public:
    TypeError(ExpPtrArg where,
              ExpPtrArg expectedType,
              ExpPtrArg actualType)
      : where(where),
        expected(expectedType),
        actual(actualType) {}

    std::string show() const override;

private:
    ExpPtr where, expected, actual;
};
