#pragma once
#include "Expression.h"

namespace Language
{

class Operator;
class Operation : public Expression
{
public:
    Operation(const std::shared_ptr<Operator>& op,
              ExpPtrArg left,
              ExpPtrArg right);
    ~Operation();

    ExpPtr evaluation(Environment*& env) const override;
    PatPtr pattern() const override;
    std::string toString() const override;
    virtual bool unwind(ExpPtr& lvalue,
                        ExpPtr& rvalue,
                        Environment*& env) override;
    bool hasNonOpVariable(Environment* env) const;
    void getAllVariables(
            std::vector<std::shared_ptr<Variable>>& variables) override;
private:
    std::shared_ptr<Operator> op;
    ExpPtr left;
    ExpPtr right;
};

}
