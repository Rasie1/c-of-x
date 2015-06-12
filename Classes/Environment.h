#pragma once
#include <vector>

namespace Language
{

class Expression;
class EnvironmentalVariable;
class Pattern;
class Environment
{
public:
    Environment(EnvironmentalVariable* data, Environment* next = nullptr);
    ~Environment();

    void clean();
    void clear();
    Environment* add(Pattern* p, Expression* e);
    Environment* pop();
    Expression* get(Pattern* p);

    static Environment* create();

private:
    Environment* loadDefaultVariables();

    EnvironmentalVariable* data;
    Environment* next;
};

}
