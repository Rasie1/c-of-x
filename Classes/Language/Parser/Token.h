#pragma once
#include <string>
#include <boost/variant.hpp>

namespace Tokens
{
    struct Identifier
    {
        std::string name;
    };

    struct IntegerData
    {

    };

    struct FloatData
    {

    };

    struct StringData
    {

    };

    struct LineBreak
    {

    };

    struct NoSpace
    {

    };

    struct Tabulation
    {
        size_t size;
    };

    struct OpeningBrace
    {

    };

    struct ClosingBrace
    {

    };

    using Token = boost::variant<
        Identifier,
        LineBreak,
        NoSpace,
        Tabulation,
        IntegerData,
        FloatData,
        StringData,
        OpeningBrace,
        ClosingBrace
    >;
}

using Token = Tokens::Token;

//enum class TokenTypeId
//{
//    Identifier = 0,
//    Data, // unroll? strings, integers, floats, etc
//    Tabulation,
//    LineBreak,
//    OpeningBrace,
//    ClosingBrace,
//    Comma,
//    NoSpace
//};

//struct Token
//{
//    TokenTypeId t;
//    std::string s;
//    int tabulationAmount = 0;
//};

//struct Token
//{
//    Token() = delete;
//};

//struct Identifier : public Token
//{
//    std::string name;
//};

//struct Tabulation : public Token
//{
//    int size;
//};

//struct LineBreak : public Token
//{
//};

//struct NoSpace : public Token
//{
//};

//struct OpeningBrace : public Token
//{
//    OpeningBrace() = delete;
//};

//struct ClosingBrace : public Token
//{
//    ClosingBrace() = delete;
//};

//struct OpeningParenthesis : public OpeningBrace
//{

//};

//struct OpeningBracket : public OpeningBrace
//{

//};

//struct OpeningCurlyBrace : public OpeningBrace
//{

//};

//struct OpeningTag : public OpeningBrace
//{
//    std::string name;
//};

//struct OpeningParenthesis : public OpeningBrace
//{

//};

//struct OpeningBracket : public OpeningBrace
//{

//};

//struct OpeningCurlyBrace : public OpeningBrace
//{

//};

//struct OpeningTag : public OpeningBrace
//{
//    std::string name;
//};

//struct ClosingParenthesis : public ClosingBrace
//{

//};

//struct ClosingBracket : public ClosingBrace
//{

//};

//struct ClosingCurlyBrace : public ClosingBrace
//{

//};

//struct ClosingTag : public ClosingBrace
//{
//    std::string name;
//};

//struct ClosingParenthesis : public ClosingBrace
//{

//};

//struct ClosingBracket : public ClosingBrace
//{

//};

//struct ClosingCurlyBrace : public ClosingBrace
//{

//};

//struct ClosingTag : public ClosingBrace
//{
//    std::string name;
//};
