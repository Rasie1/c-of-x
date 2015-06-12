#pragma once
#include "Function.h"
#include <list>
#include <string>

namespace Language
{

class List : public Expression
{
public:
    List(const std::list<Expression*>& elements);

    virtual Expression* eval(Environment* env) override;

    std::string toString() const override;

    void pushBack(Expression* e);
    void pushFront(Expression* e);

    Expression* head();
    Expression* tail();

private:
    std::list<Expression*> elements;
};

}
