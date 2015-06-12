#pragma once
#include "Error.h"
#include <string>

namespace Language
{

class TypeError : public Error
{
public:
    TypeError() {}

    Expression* eval(Environment* env);

    std::string toString() const override;
};

}
