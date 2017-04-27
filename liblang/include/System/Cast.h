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
inline auto cast<Identifier>(Environment& env,
                      const std::shared_ptr<const Expression>& e)
 -> std::shared_ptr<Identifier>
{
    return std::const_pointer_cast<Identifier>(
        std::dynamic_pointer_cast<const Identifier>(e));
}

template <>
inline auto cast<Identifier>(Environment& env, const Object& e)
 -> std::shared_ptr<Identifier>
{
    return std::dynamic_pointer_cast<Identifier>(e.expression);
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
        env.debugPrint("cast " + e->show() + " to " + std::string(typeid(T).name()) + "\n");
        env.increaseDebugIndentation();

        std::shared_ptr<T> ret;

        ret = std::dynamic_pointer_cast<T>(e);

        if (!ret)
        {
            // if (auto op = std::dynamic_pointer_cast<const Operation>(e))
            // if (typeEquals<const Intersection>(op->op))
            // {
            //     auto l = cast<T>(env, op->left);
            //     auto r = cast<T>(env, op->right);
            //     if (l && r)
            //         return l;
            // }

            auto casted = std::dynamic_pointer_cast<Expression>(e);
            auto evaluated = casted->eval(env);

            if (evaluated.expression != e)
                ret = cast<T>(env, evaluated);
        }

        if (ret == nullptr)
            env.debugPrint("unable to cast");
        else
            env.debugPrint("result: " + ret->show());
        env.decreaseDebugIndentation();

        return ret;
    }
}

template <class T>
bool checkType(Environment& env, const std::shared_ptr<const Expression>& e)
{
    return cast<const T>(env, e) != nullptr;
}

template <class T>
auto castNoEval(const std::shared_ptr<Expression>& e) 
 -> std::shared_ptr<T>
{
    return std::dynamic_pointer_cast<T>(e);
}

template <class T>
auto checkTypeNoEval(const std::shared_ptr<Expression>& e) 
 -> bool
{
    return castNoEval<T>(e) != nullptr;
}

template <class T>
auto castNoEval(const Object& e) 
 -> std::shared_ptr<T>
{
    return std::dynamic_pointer_cast<T>(e.expression);
}

template <class T>
auto checkTypeNoEval(const Object& e) 
 -> bool
{
    return castNoEval<T>(e.expression) != nullptr;
}
