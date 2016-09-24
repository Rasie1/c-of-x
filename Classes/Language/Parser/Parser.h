#pragma once
#include <string>
#include <memory>
#include <vector>
#include "Parser/Token.h"
#include "Parser/Lexer.h"

class Expression;
typedef std::shared_ptr<Expression> ExpPtr;
class Environment;
class Parser
{
public:
    ExpPtr parse(const std::string& s, Environment& env);
    ExpPtr parseFile(const std::string& filename,
                     Environment& env);
private:
    ExpPtr parse(const std::string& s,
                 size_t& i,
                 size_t n,
                 Environment& env);
    ExpPtr parseName(const std::string& s,
                     size_t i,
                     size_t n,
                     Environment& env);
    Lexer lexer;
};

