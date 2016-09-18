#pragma once
#include "Operator.h"

class Application : public Operator
{
public:
    Application();

    ExpPtr operate(ExpPtrArg first,
                   ExpPtrArg second,
                   Environment& env) const override;

    bool unapplyVariables(ExpPtrArg e, ExpPtrArg l, ExpPtrArg r, Environment &env) const override;

    std::string show() const override;
private:
};

class ReverseApplication : public Operator
{
public:
    ReverseApplication();

    ExpPtr operate(ExpPtrArg first,
                   ExpPtrArg second,
                   Environment& env) const override;

    bool unapplyVariables(ExpPtrArg e, ExpPtrArg l, ExpPtrArg r, Environment &env) const override;

    std::string show() const override;
    static const std::string defaultName;
private:
    Application proxy;
};

class LowPriorityApplication : public Operator
{
public:
    LowPriorityApplication();

    ExpPtr operate(ExpPtrArg first,
                   ExpPtrArg second,
                   Environment& env) const override;

    bool unapplyVariables(ExpPtrArg e, ExpPtrArg l, ExpPtrArg r, Environment &env) const override;

    std::string show() const override;
    static const std::string defaultName;
private:
    Application proxy;
};
