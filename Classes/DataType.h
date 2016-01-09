#pragma once
#include "Expression.h"

class DataType : public Expression
{
public:
    ExpPtr eval(Environment*& env) const override;
private:
};
