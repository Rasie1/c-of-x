#pragma once
#include <string>
#include <memory>
#include <vector>

class Expression;
class Pattern;
typedef std::shared_ptr<Expression> ExpPtr;
typedef std::shared_ptr<Pattern>    PatPtr;
class Environment;
class Parser
{
public:
    ExpPtr parse(const std::string& s, Environment* env);
    ExpPtr parseFile(const std::string& filename);
    ExpPtr parseFile(const std::string& filename,
                     Environment* env);
private:
    ExpPtr parse(const std::string& s,
                 size_t& i,
                 size_t n,
                 Environment* env);
    ExpPtr parseName(const std::string& s,
                     size_t i,
                     size_t n,
                     Environment* env);
    struct Token
    {
        std::string name;
        bool attachedToL = false;
        bool attachedToR = false;
    };
    std::vector<Token> split(const std::string& s,
                             Environment* env);
};

