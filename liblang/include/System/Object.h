#pragma once
#include <memory>
#include <typeinfo>
#include <boost/optional.hpp>
#include <vector>

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
    static constexpr Object makeObject(Args&&... args)
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

    inline bool operator==(const Object& other) const
    {
        return expression == other.expression;
    }

    inline bool operator!=(const Object& other) const
    {
        return expression != other.expression;
    }
        
    inline explicit operator bool() const
    {
        return expression.operator bool();
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
constexpr Object makeObject(Args&&... args)
{
    return Object::makeObject<T>(std::forward<Args>(args)...);
}

