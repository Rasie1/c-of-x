#include "Identifier.h"
#include "Environment.h"
#include <string>
#include <list>
#include "Error.h"

//ExpPtr Identifier::eval(Environment*& env) const
//{
//    auto ret = env->get(std::const_pointer_cast<Expression>(shared_from_this()));
//    if (ret == nullptr)
//        return make_ptr<ErrorWithMessage>("Unknown name \"" + name + "\"");
//        // return std::const_pointer_cast<Expression>(shared_from_this());

//    return ret;
//}

bool Identifier::operator==(const Expression& other) const
{
    try
    {
        auto id = dynamic_cast<const Identifier&>(other);
        return id.name == name;
    }
    catch (std::bad_cast& bc)
    {
        return false;
    }

//    if (typeid(other) == typeid(*this))
//        return name == static_cast<const Identifier&>(other).name;

}

std::string Identifier::show() const
{
    return name;
}

//bool Identifier::unwind(ExpPtr& lvalue,
//                        ExpPtr& rvalue,
//                        Environment*& env)
//{
//    auto evaluated = env->get(rvalue);
//    if (evaluated == nullptr)
//        return false;
//    rvalue = evaluated;

//    return true;
//}

bool Identifier::hasNonOpVariable(Environment* env) const
{
    return env->getEqual(std::const_pointer_cast<Expression>(shared_from_this())) == nullptr;
}
