#pragma once
#include "Predicate.h"

class IntegerType : public Predicate
{
public:
    bool holds(ExpPtrArg e, Environment* env) const override;

    std::string show() const;
    static const std::string defaultName;
};
