#pragma once
#include "DataType.h"

class Void : public DataType
{
public:
    virtual std::string show() const;
    static const std::string defaultName;
};