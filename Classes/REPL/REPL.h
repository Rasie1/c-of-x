#pragma once
#include "Environment.h"
#include "Parser/Parser.h"
#include "AutoCompleter.h"

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
    AutoCompleter completer;

    bool command(const std::string& s);
    bool shouldExit(const std::string& s);
    void addCompletionNames();
};

