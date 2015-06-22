#pragma once
#include "Pattern.h"

namespace Language
{

class Operator;
class PatternOperator : public Pattern
{
public:
    PatternOperator(const std::string& name,
                    const std::shared_ptr<Operator>& op);

    bool match(const PatPtr& other,
               Environment* env) const override;
    bool isOperator(Environment* env) const override;
    bool isMoreThan(const std::shared_ptr<PatternOperator>& other,
                    Environment* env) const;

    std::string toString() const override;

    std::string name;
    std::shared_ptr<Operator> op;
};

}
