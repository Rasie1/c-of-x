#include "Expressions/Inverse.h"
#include "Expressions/Void.h"

ExpPtr Inverse::apply(ExpPtrArg e, Environment& env) const
{
    if (auto f = d_cast<Morphism>(e))
        return f->inverse();
    else
        return make_ptr<Void>();
}

std::string Inverse::show() const
{
    return defaultName;
}

const std::string Inverse::defaultName = "inverse";

