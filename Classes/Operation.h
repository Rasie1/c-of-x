#pragma once
#include "Expression.h"

class Operator;
class Operation : public Expression
{
public:
    Operation(const std::shared_ptr<Operator>& op,
              ExpPtrArg left,
              ExpPtrArg right);

    ExpPtr evaluation(Environment*& env) const override;
    std::string show() const override;
    virtual bool unwind(ExpPtr& lvalue,
                        ExpPtr& rvalue,
                        Environment*& env);
    bool hasNonOpVariable(Environment* env) const;

    std::shared_ptr<Operator> op;
    ExpPtr left;
    ExpPtr right;
};

