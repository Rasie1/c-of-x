#pragma once
#include "Expression.h"
#include <map>
//#include <unordered_map>

struct ExpPtrLess
{
    bool operator() (ExpPtrArg l, ExpPtrArg r) const
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

    void add(ExpPtrArg key, ExpPtrArg predicate);
    void addEqual(ExpPtrArg key, ExpPtrArg value);
    ExpPtr get(ExpPtrArg key);
    ExpPtr getEqual(ExpPtrArg key);
    ExpPtr intersect(ExpPtrArg l, ExpPtrArg r);
    void clear();

    bool compareOperators(const std::shared_ptr<Operator>& first,
                          const std::shared_ptr<Operator>& second);

private:
    void addDefaultVariables();

    //std::unordered_map<ExpPtr, ExpPtr, ExpPtrHash, ExpPtrLess> data;
    std::map<ExpPtr, ExpPtr, ExpPtrLess> data;

};


