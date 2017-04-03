#pragma once
#include <memory>
#include "Expressions/Expression.h"
#include "Expressions/Operation.h"
#include "Expressions/Intersection.h"
#include "Expressions/Equality.h"
#include "Expressions/Identifier.h"
#include "System/Environment.h"
#include "System/Object.h"

template <class T>
constexpr bool typeEquals(const std::shared_ptr<const Expression>& e)
{
    return typeid(*e) == typeid(T);
}

template <class T>
constexpr bool typeEquals(const Object& e)
{
    return typeid(*e) == typeid(T);
}

namespace cast_impl
{
    template <class T>
    auto cast_helper_visitor(Environment& env, 
                             const std::shared_ptr<Expression>& e)
     -> std::shared_ptr<T>;
}



template <class T>
auto cast(Environment& env, const Object& e)
 -> std::shared_ptr<T>
{
    auto v = Identifier::unwrapIfId(e, env).expression;
    return cast_impl::cast_helper_visitor<T>(env, v);
}

template <class T>
auto cast(Environment& env, 
                   const std::shared_ptr<const Expression>& e)
 -> std::shared_ptr<T>
{
    auto v = Identifier::unwrapIfId(e, env).expression;
    return cast_impl::cast_helper_visitor<T>(env, v);    
}

template <>
auto cast<Identifier>(Environment& env, 
                      const std::shared_ptr<const Expression>& e)
 -> std::shared_ptr<Identifier>
{
    return std::const_pointer_cast<Identifier>(
        std::dynamic_pointer_cast<const Identifier>(e));
}

template <class T>
bool checkType(Environment& env, const Object& e)
{
    return cast<T>(env, e) != nullptr;
}

namespace cast_impl
{

    template <class T>
    auto cast_helper_visitor(Environment& env, 
                             const std::shared_ptr<Expression>& e)
     -> std::shared_ptr<T>
    {
        if (auto casted = std::dynamic_pointer_cast<T>(e))
            return casted;
        
        if (auto op = std::dynamic_pointer_cast<const Operation>(e))
        if (typeEquals<const Intersection>(op->op))
        {
            if (auto l = cast<T>(env, op->left))
                return l;
            if (auto r = cast<T>(env, op->right))
                return r;
        }

        auto casted = std::dynamic_pointer_cast<Expression>(e);
        auto evaluated = casted->eval(env);

        if (evaluated.expression != e) // temp
            return cast<T>(env, evaluated);
        else
            return nullptr;
    }
}

template <class T>
bool checkType(Environment& env, const std::shared_ptr<const Expression>& e)
{
    return cast<const T>(env, e) != nullptr;
}
