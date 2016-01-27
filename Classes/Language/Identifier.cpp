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

bool Identifier::match(ExpPtrArg other) const
{
    auto variable = d_cast<Identifier>(other);
    if (variable)
        return name == variable->name;

    return false;
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
    return env->get(std::const_pointer_cast<Expression>(shared_from_this())) == nullptr;
}
