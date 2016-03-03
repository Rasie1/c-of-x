#pragma once
#include "Expression.h"
#include <map>
//#include <unordered_map>

struct ExpPtrLess
{
    bool operator() (CExpPtrArg l, CExpPtrArg r) const
    {
        return *l < *r;
    }
};

//struct ExpPtrHash
//{
//  size_t operator() (ExpPtrArg key) const
//  {
//      return key->hash();
//  }
//};

class Operator;
class Environment
{
public:
    Environment();

    void add(CExpPtrArg key, ExpPtrArg predicate);
    void addEqual(CExpPtrArg key, ExpPtrArg value);
    ExpPtr get(CExpPtrArg key) const;
    ExpPtr getEqual(CExpPtrArg key) const;
    ExpPtr intersect(ExpPtrArg l, ExpPtrArg r);
    void erase(CExpPtrArg e);
    void clear();

    bool compareOperators(const std::shared_ptr<Operator>& first,
                          const std::shared_ptr<Operator>& second);

    std::string show() const;
    bool operator==(const Environment& other) const;

private:
    void addDefaultVariables();

    //std::unordered_map<ExpPtr, ExpPtr, ExpPtrHash, ExpPtrLess> data;
    std::map<CExpPtr, ExpPtr, ExpPtrLess> data;

};


