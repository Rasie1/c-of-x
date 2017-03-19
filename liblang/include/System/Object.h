#pragma once
#include <memory>
#include <typeinfo>

class Expression;

typedef std::shared_ptr<Expression> ExpPtr;
typedef const ExpPtr& ExpPtrArg;
typedef std::shared_ptr<const Expression> CExpPtr;
typedef const CExpPtr& CExpPtrArg;

using boost::optional;
using boost::none;
using boost::make_optional;
using std::static_pointer_cast;
using std::dynamic_pointer_cast;

template <class T, class... Args>
constexpr auto make_ptr(Args&&... args)
 -> decltype(std::make_shared<T>(std::forward<Args>(args)...))
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

