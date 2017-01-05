#include "System/DebugPrinter.h"
#include <iostream>

static const int indentationSize = 3;

DebugPrinter::DebugPrinter(std::ostream* stream) :
    shouldPrint(false),
    indentation(0),
    stream(stream)
{}

void DebugPrinter::toggleDebugPrint()
{
    shouldPrint = !shouldPrint;
}

void DebugPrinter::debugPrint(const std::string& s, bool shouldIndent) const
{
    if (shouldPrint)
    {
        if (shouldIndent)
        {
            for (int i = 0; i < indentation; ++i)
            {
                for (int j = 0; j < indentationSize; ++j)
                    std::cout << " ";
                std::cout << "| ";
            }
        }
        *stream << s;
        stream->flush();
    }
}

void DebugPrinter::increaseDebugIndentation()
{
    indentation++;
}

void DebugPrinter::decreaseDebugIndentation()
{
    indentation--;
}
