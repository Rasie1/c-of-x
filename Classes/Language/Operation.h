#pragma once
#include "Expression.h"

class Operator;
class Operation : public Expression
{
public:
    Operation(const std::shared_ptr<Operator>& op,
              ExpPtrArg left,
              ExpPtrArg right);

    ExpPtr eval(Environment*& env) const override;
    std::string show() const override;
    virtual bool unwind(ExpPtr& lvalue,
                        ExpPtr& rvalue,
                        Environment*& env);

    ExpPtr intersect(ExpPtrArg other, Environment *env);
    bool hasNonOpVariable(Environment* env) const;

    bool operator==(const Expression& other) const override;

    std::shared_ptr<Operator> op;
    ExpPtr left;
    ExpPtr right;
};

