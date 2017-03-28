#pragma once
#include "System/Environment.h"
#include "Parser/Parser.h"
#include "AutoCompleter.h"
#include "Input.h"

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
    Input input;

    bool command(const std::string& s);
    bool shouldExit(const std::string& s);
    void formCompletionNames();

    Object execute(const std::string& s);
};

