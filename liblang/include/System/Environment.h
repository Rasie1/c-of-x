#pragma once
#include <map>
#include <iostream>
#include <vector>
#include "Expressions/Expression.h"
#include "System/DebugPrinter.h"

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
class DebugPrinter;
class Environment
{
public:
    Environment(const std::shared_ptr<DebugPrinter>& debugPrinter = std::make_shared<DebugPrinter>(&std::cout));

    void add(CExpPtrArg key, ExpPtrArg predicate, bool excluding = true);
    void addEqual(CExpPtrArg key, ExpPtrArg value, bool excluding = true);
    // void addIntersecting(CExpPtrArg key, ExpPtrArg pred);
    void replace(CExpPtrArg key, ExpPtrArg predicate, bool excluding = true);
    void replaceEqual(CExpPtrArg key, ExpPtrArg value, bool excluding = true);
    ExpPtr get(CExpPtrArg key) const;
    ExpPtr getEqual(CExpPtrArg key) const;
    void erase(CExpPtrArg e);
    void clear();

    void increaseDebugIndentation();
    void decreaseDebugIndentation();
    void toggleDebugPrint();
    bool getDebugPrint();
    void setDebugPrint(bool enabled);
    void debugPrint(const std::string& s, bool shouldIndent = true) const;

    bool compareOperators(const std::shared_ptr<Operator>& first,
                          const std::shared_ptr<Operator>& second);

    std::string show() const;
    bool operator==(const Environment& other) const;

    std::vector<std::string> getAllNames() const;

    std::shared_ptr<Operator> defaultOperator;

private:
    void addDefaultDefinitions();

    //std::unordered_map<ExpPtr, ExpPtr, ExpPtrHash, ExpPtrLess> data;
    std::map<CExpPtr, ExpPtr, ExpPtrLess> data;

    std::shared_ptr<DebugPrinter> debugPrinter;
};
