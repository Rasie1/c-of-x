#pragma once
#include "environment.h"

namespace cx {

expression Eval(expression&& e, environment& env);
expression Apply(expression&& function, 
                 expression&& argument, 
                 environment& env);

}