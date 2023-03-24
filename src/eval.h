#pragma once
#include "environment.h"

namespace cx {

expression Eval(expression&& e, environment& env);

}