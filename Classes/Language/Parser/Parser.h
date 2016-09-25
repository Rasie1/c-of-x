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
    ExpPtr parse(const std::vector<Token>::iterator& begin,
                 const std::vector<Token>::iterator& end,
                 Environment& env);
    Lexer lexer;
};
