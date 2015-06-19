#pragma once
#include "Pattern.h"

namespace Language
{

class Operator;
class PatternOperator : public Pattern
{
public:
    PatternOperator(const std::string& name, Operator* op);

    bool match(Pattern* other, Environment* env) override;
    bool isOperator(Environment* env) override;
    bool isMoreThan(PatternOperator* other, Environment* env) const;

    std::string toString() const override;

    std::string name;
    Operator* op;
};

}
