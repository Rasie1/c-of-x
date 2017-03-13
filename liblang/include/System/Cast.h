#pragma once
#include "Expressions/Operation.h"
#include "Expressions/Intersection.h"

template <class T>
constexpr bool typeEquals(const std::shared_ptr<const Expression>& e)
{
    return typeid(*e) == typeid(T);
}

template <class T, class F>
auto d_cast(const std::shared_ptr<F>& e)
 -> decltype(std::dynamic_pointer_cast<T>(e))
{
    if (auto casted = std::dynamic_pointer_cast<T>(e))
        return casted;
    if (auto op = std::dynamic_pointer_cast<Operation>(e))
    if (typeEquals<Intersection>(op->op))
    {
        if (auto l = d_cast<T>(op->left))
            return l;
        if (auto r = d_cast<T>(op->right))
            return r;
    }

    return nullptr;
}

template <class T, class F>
auto d_cast(const std::shared_ptr<const F>& e)
 -> decltype(std::dynamic_pointer_cast<const T>(e))
{
    if (auto casted = std::dynamic_pointer_cast<const T>(e))
        return casted;
    if (auto op = std::dynamic_pointer_cast<const Operation>(e))
    if (typeEquals<const Intersection>(op->op))
    {
        if (auto l = d_cast<T>(op->left))
            return l;
        if (auto r = d_cast<T>(op->right))
            return r;
    }

    return nullptr;
}

template <class T>
constexpr bool checkType(const std::shared_ptr<const Expression>& e)
{
    // return typeid(*e) == typeid(T);
    return d_cast<const T>(e) != nullptr;
}
