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
        int data;
    };

    struct FloatData
    {

    };

    struct StringData
    {
        std::string data;
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

    struct Opening
    {

    };

    struct Closing
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
        Opening,
        Closing
    >;
}

using Token = Tokens::Token;
