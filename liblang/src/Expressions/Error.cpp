#include "Expressions/Error.h"

std::string Error::show() const
{
    return "Unknown error";
}

std::string ErrorWithMessage::show() const
{
    return "Error \"" + message + "\"";
}
