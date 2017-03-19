#pragma once
#include <string>
#include <memory>
#include <vector>
#include "Parser/Token.h"
#include "Parser/Lexer.h"

class Expression;
typedef std::shared_ptr<Expression> Object;
class Environment;
class Parser
{
public:
    Object parse(const std::string& s, Environment& env);
    Object parseFile(const std::string& filename,
                     Environment& env);
private:
    Object parse(const std::vector<Token>::iterator& begin,
                 const std::vector<Token>::iterator& end,
                 Environment& env,
                 size_t indentation = 0);
    Lexer lexer;
    std::vector<Object> previousLine;
    std::vector<Object> currentLine;
};
