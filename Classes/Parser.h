#pragma once
#include <string>
#include <stack>

namespace Language
{

class Expression;
class Parser
{
public:
    static Expression* parse(const std::string& s);
private:
    static Expression* parse(const std::string& s, size_t& i, size_t n);
    static Expression* parseName(const std::string& s, size_t& i, size_t n);
    /*Expression* parse(std::stringstream& ss, std::string s = "");
    Expression* parseInteger(const std::string& s);
    Expression* parseName(const std::string& name);
    Expression* parse(size_t i,
                      size_t n,
                      size_t& shift,
                      std::stack<Expression*>& appStack);*/

    std::string s;
};

}
