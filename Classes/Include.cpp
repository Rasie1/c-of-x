#include "Include.h"
#include <iostream>
#include <fstream>
#include "String.h"
#include "Parser.h"
#include "Environment.h"

namespace Language
{

ExpPtr Include::apply(const ExpPtr& e, Environment*& env) const
{
    auto str = std::dynamic_pointer_cast<String>(e);
    auto newEnv = env;
    if (str == nullptr)
        return Expression::apply(e, newEnv);

    auto filename = str->value;

    std::ifstream ifs(filename);
    std::string content((std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()   ));

    newEnv = env;
    return Parser::parse(content, env)->eval(newEnv);
    /*std::ifstream file(filename, std::ios::binary);

    file.seekg(0,std::ios::end);
    std::streampos size = file.tellg();
    file.seekg(0,std::ios::beg);

    std::streambuf* raw_buffer = file.rdbuf();

    char* block = new char[size];
    raw_buffer->sgetn(block, size);

    Parser::parse(block);

    delete[] block;*/

}

std::string Include::toString() const
{
    return defaultName;
}

const std::string Include::defaultName = "include";

}
