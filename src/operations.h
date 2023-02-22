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
expression SubstituteVariables(expression&& expr, environment& env);
std::pair<expression, std::optional<std::string>> FixWithVariable(expression&& expr, environment& env);

std::optional<std::string> ExtendEnvironment(
        expression&& function, 
        const expression& argument, 
        environment& env);

expression Intersect(expression&& l, expression&& r, environment& env);
bool IsError(expression& e);
expression Negate(expression&& f, environment& env);
expression Union(expression&& l, expression&& r);

}