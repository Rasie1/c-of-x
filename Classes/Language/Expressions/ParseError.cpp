#include "ParseError.h"

std::string ParseError::show() const
{
    return "Parse " + ErrorWithMessage::show();
}
