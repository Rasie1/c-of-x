#pragma once
#include "Expression.h"

class DataType : public Expression
{
public:
    ExpPtr eval(Environment& env) const override;
    //virtual bool operator==(const Expression& other) const = 0;
private:
};
