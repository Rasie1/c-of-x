#pragma once
#include "Expressions/Operation.h"
#include "Expressions/Intersection.h"
#include "Expressions/Equality.h"

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

    template <class T, class F>
    auto cast_helper_visitor(const std::shared_ptr<F>& e)
     -> decltype(std::dynamic_pointer_cast<T>(e))
    {
        if (auto casted = std::dynamic_pointer_cast<T>(e))
            return casted;
        if (auto op = std::dynamic_pointer_cast<const Operation>(e))
        if (typeEquals<const Intersection>(op->op))
        {
            if (auto l = cast_helper_visitor<T>(op->left.expression))
                return l;
            if (auto r = cast_helper_visitor<T>(op->right.expression))
                return r;
        }

        return nullptr;
    }

    template <class T, class F>
    auto cast_helper(const std::shared_ptr<F>& e)
     -> decltype(cast_helper_visitor<T>(e))
    {
        return cast_helper_visitor<T>(e);    
    }

    template <class T, class F>
    auto cast_helper(const std::shared_ptr<const F>& e)
     -> decltype(cast_helper_visitor<const T>(e))
    {
        return cast_helper_visitor<const T>(e);    
    }
}

template <class T>
auto cast(const Object& e)
 -> decltype(cast_impl::cast_helper<T>(e.expression))
{
    return cast_impl::cast_helper<T>(e.expression);    
}

template <class T, class F>
auto cast(const std::shared_ptr<F>& e)
 -> decltype(cast_impl::cast_helper_visitor<T>(e))
{
    return cast_impl::cast_helper_visitor<T>(e);    
}

template <class T, class F>
auto cast(const std::shared_ptr<const F>& e)
 -> decltype(cast_impl::cast_helper_visitor<const T>(e))
{
    return cast_impl::cast_helper_visitor<const T>(e);    
}

template <class T>
constexpr bool checkType(const Object& e)
{
    return cast<T>(e) != nullptr;
}

template <class T>
constexpr bool checkType(const std::shared_ptr<const Expression>& e)
{
    return cast<const T>(e) != nullptr;
}
