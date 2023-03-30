#pragma once
#include <optional>
#include "environment.h"
#include "io.h"

#include "apply.h"
#include "eval.h"
#include "unapply.h"
#include "equality.h"
#include "substitution.h"

namespace cx {

expression GetElement(expression&& set, environment& env);
expression Intersect(expression&& l, expression&& r, environment& env);
bool IsError(expression& e);
expression Negate(expression&& f, environment& env);
expression Union(expression&& l, expression&& r);

}