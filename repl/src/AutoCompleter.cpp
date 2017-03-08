#include "AutoCompleter.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <iostream>
#include <sstream>
#include <readline/readline.h>
#include <readline/history.h>

using namespace std;

static std::vector<std::string>* gNames = nullptr;

char *dupstr(const char* s)
{
    char *r;

    r = new char[strlen(s) + 1];
    strcpy(r, s);
    return (r);
}

char *command_generator (const char*, int);
char **fileman_completion (const char*, int, int);

char* command_generator(const char* text, int state)
{
    static int len;
    static size_t i;

    /* If this is a new word to complete, initialize now.  This
       includes saving the length of TEXT for efficiency, and
       initializing the index variable to 0. */
    if (!state) {
        len = strlen(text);
        i = 0;
    }

    /* Return the next name which partially matches from the
       command list. */
    while (i < gNames->size())
    {
        auto name = (*gNames)[i];
        i++;
        if (strncmp(name.c_str(), text, len) == 0)
            return (dupstr(name.c_str()));
    }

    /* If no names matched, then return NULL. */
    return ((char*) NULL);
}

/* Attempt to complete on the contents of TEXT.  START and END
   bound the region of rl_line_buffer that contains the word to
   complete.  TEXT is the word to complete.  We can use the entire
   contents of rl_line_buffer in case we want to do some simple
   parsing.  Returnthe array of matches, or NULL if there aren't any. */
char **fileman_completion(const char * text, int start, int end)
{
    char **matches;

    matches = rl_completion_matches(text, command_generator);

    return matches;
}


AutoCompleter::AutoCompleter()
{
    gNames = &names;
    rl_readline_name = "&";
    rl_attempted_completion_function = fileman_completion;
}
