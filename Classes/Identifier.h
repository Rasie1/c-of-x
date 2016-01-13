#pragma once
#include "DataType.h"
#include <string>

class Identifier : public DataType
{
public:
    Identifier(const std::string& name)
        : name(name) {}

//    ExpPtr eval(Environment*& env) const override;
    bool match(ExpPtrArg other) const;
    std::string show() const override;

//    virtual bool unwind(ExpPtr& lvalue,
//                        ExpPtr& rvalue,
//                        Environment*& env) override;

    bool hasNonOpVariable(Environment* env) const;

    std::string name;
};
