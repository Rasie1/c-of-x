#pragma once
#include <optional>
#include "types.h"
#include "io.h"

#include "apply.h"
#include "eval.h"
#include "unapply.h"
#include "equality.h"

namespace cx {

expression GetElement(expression&& set);
expression Fix(expression&& expr, environment& env);

std::optional<std::string> ExtendEnvironment(
        expression&& function, 
        const expression& argument, 
        environment& env);

expression Intersect(expression&& l, expression&& r);
bool IsError(expression& e);
expression Negate(expression&& f, environment& env);
expression Union(expression&& l, expression&& r);

}