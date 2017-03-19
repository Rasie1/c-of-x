#pragma once
#include "Expressions/Error.h"
#include <string>

class TypeError : public Error
{
public:
    TypeError(const Object& where,
              const Object& expectedType,
              const Object& actualType)
      : where(where),
        expected(expectedType),
        actual(actualType) {}

    std::string show() const override;

private:
    Object where, expected, actual;
};
