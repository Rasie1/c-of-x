#pragma once
#include "Environment.h"
#include "Parser/Parser.h"

class REPL
{
public:
    REPL();

    void start();
    void toggleDebugPrint();
    void loadFile(const std::string& s);
private:
    Environment env;
    Parser parser;

    bool command(const std::string& s);
    bool shouldExit(const std::string& s);
};

