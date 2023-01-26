#pragma once

namespace cx::interpreter {

void test();

}

namespace cx {

void eval(const char* code, int verbosity);
void eval_file(const char* path, int verbosity);

}