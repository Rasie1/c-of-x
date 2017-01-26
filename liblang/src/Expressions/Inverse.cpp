#include "Expressions/Inverse.h"
#include "Expressions/Void.h"
#include "Expressions/Identifier.h"

ExpPtr Inverse::apply(ExpPtrArg e, Environment& env) const
{
    auto value = Identifier::unwrapIfId(e, env);
    if (auto f = d_cast<Morphism>(value))
        return f->inverse();
    else
        return make_ptr<Void>();
}

std::string Inverse::show() const
{
    return defaultName;
}

const std::string Inverse::defaultName = "inverse";

