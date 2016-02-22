#pragma once
#include "Type.h"

class IntegerType : public Type
{
public:
    bool holds(ExpPtrArg e, Environment* env) const override;

    std::string show() const;
    static const std::string defaultName;
};
