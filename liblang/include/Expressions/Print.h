#pragma once
#include "Expressions/Morphism.h"

class Print : public Morphism
{
public:
    Print() {}

    ExpPtr apply(ExpPtrArg e,
                 Environment& env) override;
    virtual std::string show() const;
    static const std::string defaultName;
};

class PrintInfo : public Morphism
{
public:
    PrintInfo() {}

    ExpPtr apply(ExpPtrArg e,
                 Environment& env) override;
    virtual std::string show() const;
    static const std::string defaultName;
};

class PrintEnv : public Morphism
{
public:
    PrintEnv() {}

    ExpPtr apply(ExpPtrArg e,
                 Environment& env) override;
    virtual std::string show() const;
    static const std::string defaultName;
};
