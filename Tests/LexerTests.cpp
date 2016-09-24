#define BOOST_TEST_MODULE LexerTests
#include <boost/test/unit_test.hpp>
#include <iostream>
#include "Parser/Lexer.h"

BOOST_AUTO_TEST_CASE(OneIdentifier)
{
    Lexer l;
    l.tokenize("one");
    auto tokens = l.getTokens();

    BOOST_REQUIRE_EQUAL(tokens.size(), 1);

    auto id = tokens[0];
    BOOST_CHECK(id.t == TokenTypeId::Identifier);
    BOOST_CHECK_EQUAL(id.s, "one");
}

BOOST_AUTO_TEST_CASE(IdentifierWithSpaces)
{
    Lexer l;
    l.tokenize(" id ");
    auto tokens = l.getTokens();

    BOOST_REQUIRE_EQUAL(tokens.size(), 1);

    auto id = tokens[0];
    BOOST_CHECK(id.t == TokenTypeId::Identifier);
    BOOST_CHECK_EQUAL(id.s, "id");
}

BOOST_AUTO_TEST_CASE(TwoIdentifiers)
{
    Lexer l;
    l.tokenize("f x");
    auto tokens = l.getTokens();

    BOOST_REQUIRE_EQUAL(tokens.size(), 2);

    auto f = tokens[0];
    auto x = tokens[1];
    BOOST_CHECK(f.t == TokenTypeId::Identifier);
    BOOST_CHECK(x.t == TokenTypeId::Identifier);
    BOOST_CHECK_EQUAL(f.s, "f");
    BOOST_CHECK_EQUAL(x.s, "x");
}

BOOST_AUTO_TEST_CASE(ManyIdentifiers)
{
    Lexer l;
    l.tokenize("asdf sa fdf as  sdf a   sd  d da       ");
    auto tokens = l.getTokens();

    BOOST_REQUIRE_EQUAL(tokens.size(), 9);

    auto asdf = tokens[0];
    auto sa = tokens[1];
    auto fdf = tokens[2];
    auto as = tokens[3];
    auto sdf = tokens[4];
    auto a = tokens[5];
    auto sd = tokens[6];
    auto d = tokens[7];
    auto da = tokens[8];
    BOOST_CHECK(asdf.t == TokenTypeId::Identifier);
    BOOST_CHECK(sa.t == TokenTypeId::Identifier);
    BOOST_CHECK(fdf.t == TokenTypeId::Identifier);
    BOOST_CHECK(as.t == TokenTypeId::Identifier);
    BOOST_CHECK(sdf.t == TokenTypeId::Identifier);
    BOOST_CHECK(a.t == TokenTypeId::Identifier);
    BOOST_CHECK(sd.t == TokenTypeId::Identifier);
    BOOST_CHECK(d.t == TokenTypeId::Identifier);
    BOOST_CHECK(da.t == TokenTypeId::Identifier);
    BOOST_CHECK_EQUAL(asdf.s, "asdf");
    BOOST_CHECK_EQUAL(sa.s, "sa");
    BOOST_CHECK_EQUAL(fdf.s, "fdf");
    BOOST_CHECK_EQUAL(as.s, "as");
    BOOST_CHECK_EQUAL(sdf.s, "sdf");
    BOOST_CHECK_EQUAL(a.s, "a");
    BOOST_CHECK_EQUAL(sd.s, "sd");
    BOOST_CHECK_EQUAL(d.s, "d");
    BOOST_CHECK_EQUAL(da.s, "da");
}

BOOST_AUTO_TEST_CASE(LineBreak)
{
    Lexer l;
    l.tokenize("a\nb");
    auto tokens = l.getTokens();

    BOOST_REQUIRE_EQUAL(tokens.size(), 3);

    auto a = tokens[0];
    auto n = tokens[1];
    auto b = tokens[2];
    BOOST_CHECK(a.t == TokenTypeId::Identifier);
    BOOST_CHECK(n.t == TokenTypeId::LineBreak);
    BOOST_CHECK(b.t == TokenTypeId::Identifier);
    BOOST_CHECK_EQUAL(a.s, "a");
    BOOST_CHECK_EQUAL(b.s, "b");
}

BOOST_AUTO_TEST_CASE(DoubleLineBreakNegation)
{
    Lexer l;
    l.tokenize("a\n\nb");
    auto tokens = l.getTokens();

    BOOST_REQUIRE_EQUAL(tokens.size(), 3);

    auto a = tokens[0];
    auto n = tokens[1];
    auto b = tokens[2];
    BOOST_CHECK(a.t == TokenTypeId::Identifier);
    BOOST_CHECK(n.t == TokenTypeId::LineBreak);
    BOOST_CHECK(b.t == TokenTypeId::Identifier);
    BOOST_CHECK_EQUAL(a.s, "a");
    BOOST_CHECK_EQUAL(b.s, "b");
}

BOOST_AUTO_TEST_CASE(LineBreakNegationWithIndentation)
{
    Lexer l;
    l.tokenize("a\n    \n\n            \nb");
    auto tokens = l.getTokens();

    BOOST_REQUIRE_EQUAL(tokens.size(), 3);

    auto a = tokens[0];
    auto n = tokens[1];
    auto b = tokens[2];
    BOOST_CHECK(a.t == TokenTypeId::Identifier);
    BOOST_CHECK(n.t == TokenTypeId::LineBreak);
    BOOST_CHECK(b.t == TokenTypeId::Identifier);
    BOOST_CHECK_EQUAL(a.s, "a");
    BOOST_CHECK_EQUAL(b.s, "b");
}

BOOST_AUTO_TEST_CASE(XPlusY)
{
    Lexer l;
    l.tokenize("x + y");
    auto tokens = l.getTokens();

    BOOST_REQUIRE_EQUAL(tokens.size(), 3);

    auto x = tokens[0];
    auto p = tokens[1];
    auto y = tokens[2];
    BOOST_CHECK(x.t == TokenTypeId::Identifier);
    BOOST_CHECK(p.t == TokenTypeId::Identifier);
    BOOST_CHECK(y.t == TokenTypeId::Identifier);
    BOOST_CHECK_EQUAL(x.s, "x");
    BOOST_CHECK_EQUAL(p.s, "+");
    BOOST_CHECK_EQUAL(y.s, "y");
}

BOOST_AUTO_TEST_CASE(XPlusYNoSpaces)
{
    Lexer l;
    l.tokenize("x+y");
    auto tokens = l.getTokens();

    BOOST_REQUIRE_EQUAL(tokens.size(), 5);

    auto x = tokens[0];
    auto ns1 = tokens[1];
    auto p = tokens[2];
    auto ns2 = tokens[3];
    auto y = tokens[4];
    BOOST_CHECK(x.t == TokenTypeId::Identifier);
    BOOST_CHECK(ns1.t == TokenTypeId::NoSpace);
    BOOST_CHECK(p.t == TokenTypeId::Identifier);
    BOOST_CHECK(ns2.t == TokenTypeId::NoSpace);
    BOOST_CHECK(y.t == TokenTypeId::Identifier);
    BOOST_CHECK_EQUAL(x.s, "x");
    BOOST_CHECK_EQUAL(p.s, "+");
    BOOST_CHECK_EQUAL(y.s, "y");
}

BOOST_AUTO_TEST_CASE(Indentation1)
{
    Lexer l;
    l.tokenize("f\n    x");
    auto tokens = l.getTokens();

    BOOST_REQUIRE_EQUAL(tokens.size(), 5);

    auto f = tokens[0];
    auto n = tokens[1];
    auto x = tokens[2];
    auto t = tokens[3];
    BOOST_CHECK(f.t == TokenTypeId::Identifier);
    BOOST_CHECK(n.t == TokenTypeId::LineBreak);
    BOOST_CHECK(x.t == TokenTypeId::Identifier);
    BOOST_CHECK(t.t == TokenTypeId::Tabulation);
    BOOST_CHECK_EQUAL(f.s, "f");
    BOOST_CHECK_EQUAL(x.s, "x");
}
