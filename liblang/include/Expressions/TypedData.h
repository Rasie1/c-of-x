#pragma once
#include "Data.h"
#include "Expressions/ValueInSet.h"
#include "Expressions/Void.h"
#include "System/Cast.h"

template<typename Type>
class TypedData : public Data
{
public:
    Object intersect(const Object& other, Environment& env) override
    {
        if (*other == *this)
            return thisObject();
        if (auto valueInSet = cast<ValueInSet>(env, other))
            if (cast<Type>(env, valueInSet->set))
                return thisObject();

        return makeObject<Void>();
    }
};
