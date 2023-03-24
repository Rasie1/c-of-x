#pragma once
#include "environment.h"

namespace cx {


expression Eval(expression&& e, environment& env);

struct unapply_result {
    bool success;
    std::string outerVariable;

    operator bool() const {
        return success;
    }
};

unapply_result Unapply(expression&& pattern, 
                       expression&& match, 
                       environment& env);


}