#pragma once
#include <optional>
#include "types.h"
#include "util.h"
#include "io.h"

namespace cx {

expression Eval(expression&& e, environment& env);
expression GetElement(expression&& set);
expression Fix(expression&& expr, environment& env);

std::optional<std::string> ExtendEnvironment(
        expression&& function, 
        const expression& argument, 
        environment& env);
expression Equals(expression&& l,
                  expression&& r,
                  environment& env);

expression Intersect(expression&& l, expression&& r);
bool IsError(expression& e);
expression Apply(expression&& function, 
                 expression&& argument, 
                 environment& env);
bool Unapply(expression&& pattern, 
             expression&& match, 
             environment& env);
expression Eval(expression&& e, 
                environment& env);

}