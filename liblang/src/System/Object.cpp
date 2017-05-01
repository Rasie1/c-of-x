#include "System/Object.h"
#include "Expressions/Void.h"

Object::Object(const std::shared_ptr<Expression>& e) :
    expression(e ? e : std::make_shared<Void>())
{
}
