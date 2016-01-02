#pragma once
#include <string>
#include <stack>
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
    struct Token
    {
        std::string name;
        bool attachedToL = false;
        bool attachedToR = false;
    };
    static std::vector<Token> split(const std::string& s,
                                    Environment* env);
};

