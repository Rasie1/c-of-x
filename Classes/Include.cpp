#include "Include.h"
#include <iostream>
#include <fstream>
#include "String.h"
#include "Parser.h"
#include "Environment.h"

namespace Language
{

Expression* Include::apply(Expression* e, Environment*& env)
{
    auto str = dynamic_cast<String*>(e);
    if (str == nullptr)
        return Expression::applyConstEnv(e, env);

    auto filename = str->value;


    std::ifstream ifs(filename);
    std::string content((std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()   ));

    return Parser::parse(content, env)->evalConstEnv(env);
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
