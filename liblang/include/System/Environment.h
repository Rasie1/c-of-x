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

    void add(const Object& key, const Object& predicate, bool excluding = true);
    void addEqual(const Object& key, const Object& value, bool excluding = true);
    // void addIntersecting(const Object& key, const Object& pred);
    void replace(const Object& key, const Object& predicate, bool excluding = true);
    void replaceEqual(const Object& key, const Object& value, bool excluding = true);
    Object get(const Object& key) const;
    Object getEqual(const Object& key) const;
    void erase(const Object& e);
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

    //std::unordered_map<Object, Object, ObjectHash, ObjectLess> data;
    std::map<const Object, Object, ObjectLess> data;

    std::shared_ptr<DebugPrinter> debugPrinter;
};
