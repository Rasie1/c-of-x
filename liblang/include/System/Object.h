#pragma once
#include <memory>
#include <typeinfo>

class Expression;

// typedef std::shared_ptr<Expression> Object;

class Object
{
private:

public:
    Object() {}
    
    Object(const std::shared_ptr<Expression>& e) :
        expression(e)
    {
    }

    template <class T, class... Args>
    constexpr auto makeObject(Args&&... args)
     -> decltype(std::make_shared<T>(std::forward<Args>(args)...))
    {
        Object ret;
        ret.expression = std::make_shared<T>(std::forward<Args>(args)...);

        return ret;
    }

    inline Expression& operator*() const
    {
        return *expression;
    }

    inline Expression* operator->() const
    {
        return expression.operator->();
    }

    inline bool operator==(const Object& other)
    {
        return expression == other.expression;
    }

    std::shared_ptr<Expression> expression;
private:
};

using boost::optional;
using boost::none;
using boost::make_optional;
using std::static_pointer_cast;
using std::dynamic_pointer_cast;

template <class T, class... Args>
constexpr auto makeObject(Args&&... args)
 -> decltype(std::make_shared<T>(std::forward<Args>(args)...))
{
    return Object::makeObject<T>(std::forward<Args>(args)...);
}

