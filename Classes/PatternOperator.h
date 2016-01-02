#pragma once
#include "Pattern.h"

class Operator;
class PatternOperator : public Pattern
{
public:
    PatternOperator(const std::string& name,
                    const std::shared_ptr<Operator>& op);

    bool match(ExpPtrArg other) const override;
    bool isOperator(Environment* env) const override;
    bool isMoreThan(const std::shared_ptr<PatternOperator>& other,
                    Environment* env) const;

    std::string show() const override;

    std::string name;
    std::shared_ptr<Operator> op;
};

class PatternOperation : public Pattern
{
public:
    PatternOperation(PatPtrArg op,
                     PatPtrArg left,
                     PatPtrArg right);

    bool match(ExpPtrArg other) const override;
    std::string show() const override;

    PatPtr op;
    PatPtr left;
    PatPtr right;
};

