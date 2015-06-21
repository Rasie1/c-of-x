#pragma once
#include "Pattern.h"

namespace Language
{

class Operator;
class PatternOperator : public Pattern
{
public:
    PatternOperator(const std::string& name, Operator* op);

    bool match(const std::shared_ptr<Pattern>& other,
               Environment* env) const override;
    bool isOperator(Environment* env) override;
    bool isMoreThan(const std::shared_ptr<PatternOperator>& other,
                    Environment* env) const;

    std::string toString() const override;

    std::string name;
    Operator* op;
};

}
