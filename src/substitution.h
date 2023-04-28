#pragma once
#include <optional>
#include "environment.h"
#include "io.h"

#include "apply.h"
#include "eval.h"
#include "unapply.h"
#include "equality.h"

namespace cx {

expression SubstituteVariables(
    expression&& expr, 
    environment& env,
    bool prohibitFreeVariables = false);
    
std::pair<expression, std::optional<std::string>> FixWithVariable(expression&& expr, environment& env);

std::optional<std::string> ExtendEnvironment(
    expression&& function, 
    const expression& argument, 
    environment& env);

}