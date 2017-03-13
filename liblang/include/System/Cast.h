#pragma once
#include "Expressions/Operation.h"
#include "Expressions/Intersection.h"

template <class T, class... Args>
constexpr auto d_cast(Args&&... args)
 -> decltype(std::dynamic_pointer_cast<T>(std::forward<Args>(args)...))
{
    return std::dynamic_pointer_cast<T>(std::forward<Args>(args)...);
}

template <class T>
constexpr bool typeEquals(const std::shared_ptr<const Expression>& e)
{
    return typeid(*e) == typeid(T);
}

// template <class T, class... Args>
// auto d_cast(Args&&... args)
//  -> decltype(std::dynamic_pointer_cast<T>(std::forward<Args>(args)...))
// {
//     if (auto casted = std::dynamic_pointer_cast<T>(std::forward<Args>(args)...))
//         return casted;
//     if (auto op = std::dynamic_pointer_cast<Operation>(std::forward<Args>(args)...))
//     if (typeEquals<Intersection>(op->op))
//     {
//         if (auto l = d_cast<T>(op->left))
//             return l;
//         if (auto r = d_cast<T>(op->right))
//             return r;
//     }

//     // return std::dynamic_pointer_cast<T>(std::forward<Args>(args)...);

//     return nullptr;
// }

template <class T>
constexpr bool checkType(const std::shared_ptr<const Expression>& e)
{
    return typeid(*e) == typeid(T);
    // return d_cast<const T>(e) != nullptr;
}
