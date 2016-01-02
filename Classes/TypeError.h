#pragma once
#include "Error.h"
#include <string>

namespace Language
{

class TypeError : public Error
{
public:
    TypeError() {}

    Expression* evalConstEnv(Environment* env);

    std::string show() const override;
};

}
