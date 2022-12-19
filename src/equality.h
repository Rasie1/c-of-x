#pragma once
#include "types.h"

namespace cx {

expression IsEqual(expression&& l, expression&& r, environment& env);
expression Equals(expression&& l, expression&& r, environment& env);

}