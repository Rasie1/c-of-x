#pragma once
#include "types.h"

namespace cx {

std::string Show(expression&& e);
void DebugPrint(const std::string& msg, expression e, environment& env, int color = 1);
expression Print(expression&& e, environment& env);
expression Read(expression&& e, environment& env);
expression SetTraceEnabled(expression&& e, environment& env);

}