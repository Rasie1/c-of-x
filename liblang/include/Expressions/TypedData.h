#pragma once
#include "Data.h"
#include "Expressions/ValueInSet.h"
#include "Expressions/Void.h"
#include "System/Cast.h"
#include "Expressions/Equality.h"
#include "Expressions/Operation.h"

template<typename Type>
class TypedData : public Data
{
public:
    Object intersect(const Object& other, Environment& env) override
    {
        if (auto otherCasted = cast<TypedData<Type>>(env, other))
        if (*otherCasted == *this)
            return thisObject();
            
        if (auto valueInSet = cast<ValueInSet>(env, other))
        if (cast<Type>(env, valueInSet->set))
            return thisObject();

        return makeObject<Void>();
    }

    Object equals(const Object& other, Environment& env) override
    {
        if (auto i = cast<TypedData<Type>>(env, other))
        {
            if (*this == *i)
                return thisObject();
            else
                return makeObject<Void>();
        }
        return makeOperation<Equality>(thisObject(), other);
    }
};
