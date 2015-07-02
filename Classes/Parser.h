#pragma once
#include <string>
#include <stack>
#include <memory>

namespace Language
{

class Expression;
class Pattern;
typedef std::shared_ptr<Expression> ExpPtr;
typedef std::shared_ptr<Pattern>    PatPtr;
class Environment;
class Parser
{
public:
    static ExpPtr parse(const std::string& s, Environment* env);
    static ExpPtr parseFile(const std::string& filename);
    static ExpPtr parseFile(const std::string& filename,
                            Environment* env);
private:
    static ExpPtr parse(const std::string& s,
                        size_t& i,
                        size_t n,
                        Environment* env);
    static ExpPtr parseName(const std::string& s,
                            size_t i,
                            size_t n,
                            Environment* env);
};

}
