#pragma once
#include "Expressions/Predicate.h"

class Type : public Predicate
{
public:
    virtual std::string show() const;
    static const std::string defaultName;
};
