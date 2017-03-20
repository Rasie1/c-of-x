#pragma once
#include <map>
#include <iostream>
#include <vector>
#include "Expressions/Expression.h"
#include "System/DebugPrinter.h"

struct ObjectLess
{
    bool operator() (const Object& l, const Object& r) const
    {
        return *l < *r;
    }
};

//struct ObjectHash
//{
//  size_t operator() (const Object& key) const
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

    void add(const std::string& key, const Object& predicate, bool excluding = true);
    void addEqual(const std::string& key, const Object& value, bool excluding = true);
    // void addIntersecting(const std::string& key, const Object& pred);
    void replace(const std::string& key, const Object& predicate, bool excluding = true);
    void replaceEqual(const std::string& key, const Object& value, bool excluding = true);
    Object get(const std::string& key) const;
    Object getEqual(const std::string& key) const;
    void erase(const std::string& key);
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

    std::map<std::string, Object> data;

    std::shared_ptr<DebugPrinter> debugPrinter;
};
