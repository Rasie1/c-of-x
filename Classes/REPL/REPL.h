#pragma once
#include "Environment.h"
#include "Parser.h"

class REPL
{
public:
    void start();
private:
    Environment env;
    Parser parser;

    bool command(const std::string& s);
    bool shouldExit(const std::string& s);
};

