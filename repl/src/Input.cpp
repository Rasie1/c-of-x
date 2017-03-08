#include "Input.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>

using namespace std;


int upBind(int count, int key) 
{
    auto entry = previous_history();
    auto line = entry->line;

    if (line == nullptr)
        return 0;

    printf("%s", line);

    fflush(stdout);
}

int downBind(int count, int key) 
{
    auto entry = next_history();
    auto line = entry->line;

    if (line == nullptr)
        return 0;

    printf("%s", line);

    fflush(stdout);
}

int leftBind(int count, int key) 
{
    printf("\033[1A");
    fflush(stdout);
}

int rightBind(int count, int key) 
{
    printf("\f");
    fflush(stdout);
}

bool seenLineBreak = false; 

int nBind(int count, int key) 
{
    if (seenLineBreak)
    {
        printf("\n");
        fflush(stdout);
        rl_done = true;
    }
    else
    {
        printf("\n > ");
        fflush(stdout);
        seenLineBreak = true;
    }

    // stringstream code;

    // bool seenLineBreak = false; 

    // auto getLineStartString = [&seenLineBreak]()
    // {
    //     if (seenLineBreak)
    //         return " > ";
    //     else
    //         return "&> ";
    // };

    // while (true)
    // {
    //     std::string line = ::readline(getLineStartString());
    //     if (line == "")
    //     {
    //         break;
    //     }
    //     else
    //     {
    //         code << line << endl;
    //         seenLineBreak = true;
    //     }
    // }

    fflush(stdout);
}

static void setupMultilineInput()
{
    rl_bind_keyseq("\\e[A", upBind);
    rl_bind_keyseq("\\e[B", downBind);
    rl_bind_keyseq("\\e[D", leftBind);
    rl_bind_keyseq("\\e[C", rightBind);
    rl_bind_key('\r', nBind);
}

Input::Input()
{
    setupMultilineInput();
}

std::vector<std::string>& Input::completerNames()
{
    return completer.names;
}


std::string Input::read()
{
    // stringstream code;

    // if (multilineInput) 
    // {
    //     bool seenLineBreak = false; 

    //     auto getLineStartString = [&seenLineBreak]()
    //     {
    //         if (seenLineBreak)
    //             return " > ";
    //         else
    //             return "&> ";
    //     };

    //     while (true)
    //     {
    //         std::string line = ::readline(getLineStartString());
    //         if (line == "")
    //         {
    //             break;
    //         }
    //         else
    //         {
    //             code << line << endl;
    //             seenLineBreak = true;
    //         }
    //     }
    // }
    // else
    seenLineBreak = false;
    auto str = ::readline("&> ");

    add_history(str);

    return str;
}

