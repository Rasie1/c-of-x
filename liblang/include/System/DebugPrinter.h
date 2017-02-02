#pragma once
#include <iosfwd>

class DebugPrinter
{
public:
    DebugPrinter(std::ostream* stream);

    void increaseDebugIndentation();
    void decreaseDebugIndentation();
    void toggleDebugPrint();
    bool getDebugPrint();
    void setDebugPrint(bool enabled);
    void debugPrint(const std::string& s, bool shouldIndent = false) const;

private:
    bool shouldPrint;
    int indentation;
    std::ostream* stream;
};
