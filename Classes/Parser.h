#pragma once
#include <string>
#include <stack>

namespace Language
{

class Expression;
class Environment;
class Parser
{
public:
    static Expression* parse(const std::string& s, Environment* env);
private:
    static Expression* parse(const std::string& s,
                             size_t& i,
                             size_t n,
                             Environment* env);
    static Expression* parseName(const std::string& s,
                                 size_t& i,
                                 size_t n,
                                 Environment* env);
};

}
