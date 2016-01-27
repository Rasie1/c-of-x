#pragma once
#include "Function.h"

class Print : public Function
{
public:
    Print() {}

    ExpPtr apply(ExpPtrArg e,
                 Environment*& env) const override;
    virtual std::string show() const;
    static const std::string defaultName;
};
