#pragma once
#include "types.h"

namespace cx {


expression Eval(expression&& e, environment& env);

struct unapply_result {
    bool success;
    std::string conflictingVariable;

    operator bool() const {
        return success;
    }
};

unapply_result Unapply(expression&& pattern, 
                       expression&& match, 
                       environment& env);


}