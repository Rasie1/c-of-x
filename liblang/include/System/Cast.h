#pragma once
#include <memory>
#include "Expressions/Expression.h"
#include "Expressions/Operation.h"
#include "Expressions/Intersection.h"
#include "Expressions/Equality.h"
#include "System/Environment.h"

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

template <class T>
auto cast(Environment& env, const Object& e)
 -> std::shared_ptr<T>;

template <class T, class F>
auto cast(Environment& env, const std::shared_ptr<F>& e)
 -> std::shared_ptr<T>;

template <class T, class F>
auto cast(Environment& env, const std::shared_ptr<const F>& e)
 -> std::shared_ptr<const T>;

namespace cast_impl
{

    template <class T, class F>
    auto cast_helper_visitor(Environment& env, const std::shared_ptr<F>& e)
     -> decltype(std::dynamic_pointer_cast<T>(e))
    {
        auto evaluate = [&env](auto& e)
        {
            auto evaluated = 
                std::const_pointer_cast<Expression>(e)->eval(env);

            if (evaluated.expression != e) // temp
                return cast<T>(env, evaluated);
        };

        if (auto casted = std::dynamic_pointer_cast<T>(e))
            return casted;
        
        if (auto op = std::dynamic_pointer_cast<const Operation>(e))
        if (typeEquals<const Intersection>(op->op))
        {
            if (auto l = cast_helper_visitor<T>(env, op->left.expression))
                return l;
            if (auto r = cast_helper_visitor<T>(env, op->right.expression))
                return r;
        }



        return nullptr;
    }

    template <class T, class F>
    auto cast_helper(Environment& env, const std::shared_ptr<F>& e)
     -> decltype(cast_helper_visitor<T>(env, e))
    {
        return cast_helper_visitor<T>(env, e);    
    }

    template <class T, class F>
    auto cast_helper(Environment& env, const std::shared_ptr<const F>& e)
     -> decltype(cast_helper_visitor<const T>(env, e))
    {
        return cast_helper_visitor<const T>(env, e);    
    }
}

template <class T>
auto cast(Environment& env, const Object& e)
 -> std::shared_ptr<T>
{
    return cast_impl::cast_helper<T>(env, e.expression);    
}

template <class T, class F>
auto cast(Environment& env, const std::shared_ptr<F>& e)
 -> std::shared_ptr<T>
{
    return cast_impl::cast_helper_visitor<T>(env, e);    
}

template <class T, class F>
auto cast(Environment& env, const std::shared_ptr<const F>& e)
 -> std::shared_ptr<const T>
{
    return cast_impl::cast_helper_visitor<const T>(env, e);    
}

template <class T>
bool checkType(Environment& env, const Object& e)
{
    return cast<T>(env, e) != nullptr;
}

template <class T>
bool checkType(Environment& env, const std::shared_ptr<const Expression>& e)
{
    return cast<const T>(env, e) != nullptr;
}
