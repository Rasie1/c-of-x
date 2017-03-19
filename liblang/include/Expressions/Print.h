#pragma once
#include "Expressions/Morphism.h"

class Print : public Morphism
{
public:
    Print() {}

    Object apply(const Object& e,
                 Environment& env) override;
    virtual std::string show() const;
    static const std::string defaultName;
};

class PrintInfo : public Morphism
{
public:
    PrintInfo() {}

    Object apply(const Object& e,
                 Environment& env) override;
    virtual std::string show() const;
    static const std::string defaultName;
};

class PrintEnv : public Morphism
{
public:
    PrintEnv() {}

    Object apply(const Object& e,
                 Environment& env) override;
    virtual std::string show() const;
    static const std::string defaultName;
};
