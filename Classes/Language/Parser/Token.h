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
