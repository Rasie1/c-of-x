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
        double data;
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

    struct IndentationNode
    {

    };

    struct Opening
    {

    };

    struct Closing
    {

    };

    typedef boost::variant<
        Identifier,
        LineBreak,
        NoSpace,
        Tabulation,
        IntegerData,
        FloatData,
        StringData,
        Opening,
        Closing
    > Token;

    inline std::ostream& operator<<(std::ostream& stream, const Tokens::Identifier& x)
    {
        stream << "id(" << x.name << ")";
        return stream;
    }

    inline std::ostream& operator<<(std::ostream& stream, const Tokens::LineBreak&/* x*/)
    {
        stream << "LineBreak";
        return stream;
    }

    inline std::ostream& operator<<(std::ostream& stream, const Tokens::NoSpace&/* x*/)
    {
        stream << "NoSpace";
        return stream;
    }

    inline std::ostream& operator<<(std::ostream& stream, const Tokens::Tabulation& x)
    {
        stream << "Tabulation(" << x.size << ")";
        return stream;
    }

    inline std::ostream& operator<<(std::ostream& stream, const Tokens::IntegerData& x)
    {
        stream << "id(" << x.data << ")";
        return stream;
    }

    inline std::ostream& operator<<(std::ostream& stream, const Tokens::FloatData& x)
    {
        stream << "id(" << x.data << ")";
        return stream;
    }

    inline std::ostream& operator<<(std::ostream& stream, const Tokens::StringData& x)
    {
        stream << "id(" << x.data << ")";
        return stream;
    }

    inline std::ostream& operator<<(std::ostream& stream, const Tokens::Opening&/* x*/)
    {
        stream << "Opening";
        return stream;
    }

    inline std::ostream& operator<<(std::ostream& stream, const Tokens::Closing&/* x*/)
    {
        stream << "Closing";
        return stream;
    }

}

using Token = Tokens::Token;
