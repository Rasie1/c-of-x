#include "Operation.h"
#include "Operator.h"
#include "PatternOperator.h"

namespace Language
{

Operation::Operation(Operator* op, Expression* left, Expression* right)
    : op(op),
      left(left),
      right(right)
{

}

Operation::~Operation()
{
    delete op;
    delete left;
    delete right;
}

Expression* Operation::eval(Environment*& env)
{
    return op->operate(left, right, env);
}

Pattern* Operation::pattern()
{
    return new PatternOperator(op->name,
                               op);
}

std::string Operation::toString() const
{
    return "(" +
            left->toString() +
            " " + op->toString() + " " +
            right->toString() +
            ")";
}


}
