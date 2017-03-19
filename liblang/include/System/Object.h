#pragma once
#include <memory>
#include <typeinfo>

class Expression;

typedef std::shared_ptr<Expression> Object;

using boost::optional;
using boost::none;
using boost::make_optional;
using std::static_pointer_cast;
using std::dynamic_pointer_cast;

template <class T, class... Args>
constexpr auto makeObject(Args&&... args)
 -> decltype(std::make_shared<T>(std::forward<Args>(args)...))
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

