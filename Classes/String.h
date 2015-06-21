#pragma once
#include "BaseType.h"
#include <string>

namespace Language
{

class AdditionOfValue;

class String : public BaseType
{
public:
    String(const std::string& value);

    std::shared_ptr<Pattern> pattern() const override;
    std::string toString() const override;


    std::string value;
};

}
