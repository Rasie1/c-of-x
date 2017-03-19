#pragma once
#include "Expressions/Operator.h"

class Application : public Operator
{
public:
    Application();

    Object operate(const Object& first,
                   const Object& second,
                   Environment& env) override;

    bool unapplyVariables(const Object& e, const Object& l, const Object& r, Environment &env) override;

    std::string show() const override;
private:
};

class ReverseApplication : public Operator
{
public:
    ReverseApplication();

    Object operate(const Object& first,
                   const Object& second,
                   Environment& env) override;

    bool unapplyVariables(const Object& e, const Object& l, const Object& r, Environment &env) override;

    std::string show() const override;
    static const std::string defaultName;
private:
    Application proxy;
};

class LowPriorityApplication : public Operator
{
public:
    LowPriorityApplication();

    Object operate(const Object& first,
                   const Object& second,
                   Environment& env) override;

    bool unapplyVariables(const Object& e, const Object& l, const Object& r, Environment &env) override;

    std::string show() const override;
    static const std::string defaultName;
private:
    Application proxy;
};
