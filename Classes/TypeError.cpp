#include "TypeError.h"

std::string TypeError::show() const
{
    return "(Typing Error: in expression \"" +
            where->show() +
            "\", expected: \"" +
            expected->show() +
            "\", actual: \"" +
            actual->show() +
            "\")";
}
