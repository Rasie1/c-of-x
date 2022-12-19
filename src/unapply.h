#pragma once
#include "types.h"

namespace cx {

expression Eval(expression&& e, environment& env);

bool Unapply(expression&& pattern, 
             expression&& match, 
             environment& env);

}