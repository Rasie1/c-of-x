#pragma once
#include "Predicate.h"

class Type : public Predicate
{
public:
    virtual std::string show() const;
    static const std::string defaultName;
};
