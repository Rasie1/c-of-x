#define BOOST_TEST_MODULE LexerTests
#include <boost/test/unit_test.hpp>
#include <iostream>
#include "Lexer.h"

using namespace boost;
using namespace std;
using namespace Tokens;

BOOST_AUTO_TEST_CASE(OneIdentifier)
{
    Lexer l;
    l.tokenize("one");
    auto tokens = l.getTokens();

    BOOST_CHECK_EQUAL(tokens.size(), 1);

    BOOST_CHECK(tokens[0].type() == typeid(Identifier));
    auto id = get<Identifier>(tokens[0]);
    BOOST_CHECK_EQUAL(id.name, "one");
}

BOOST_AUTO_TEST_CASE(IdentifierWithSpaces)
{
    Lexer l;
    l.tokenize(" id ");
    auto tokens = l.getTokens();

    BOOST_CHECK_EQUAL(tokens.size(), 1);

    BOOST_CHECK(tokens[0].type() == typeid(Identifier));
    auto id = get<Identifier>(tokens[0]);
    BOOST_CHECK_EQUAL(id.name, "id");
}

BOOST_AUTO_TEST_CASE(TwoIdentifiers)
{
    Lexer l;
    l.tokenize("f x");
    auto tokens = l.getTokens();

    BOOST_CHECK_EQUAL(tokens.size(), 2);

    BOOST_CHECK(tokens[0].type() == typeid(Identifier));
    auto f = get<Identifier>(tokens[0]);
    BOOST_CHECK_EQUAL(f.name, "f");

    BOOST_CHECK(tokens[1].type() == typeid(Identifier));
    auto x = get<Identifier>(tokens[1]);
    BOOST_CHECK_EQUAL(x.name, "x");
}

BOOST_AUTO_TEST_CASE(SimpleBracesTest)
{
    Lexer l;
    l.tokenize("(f x)");
    auto tokens = l.getTokens();

    BOOST_CHECK_EQUAL(tokens.size(), 4);

    BOOST_CHECK(tokens[0].type() == typeid(Opening));
    BOOST_CHECK(tokens[3].type() == typeid(Closing));
    BOOST_CHECK(tokens[1].type() == typeid(Identifier));
    auto f = get<Identifier>(tokens[1]);
    BOOST_CHECK_EQUAL(f.name, "f");

    BOOST_CHECK(tokens[2].type() == typeid(Identifier));
    auto x = get<Identifier>(tokens[2]);
    BOOST_CHECK_EQUAL(x.name, "x");
}

BOOST_AUTO_TEST_CASE(ManyIdentifiers)
{
    Lexer l;
    l.tokenize("asdf sa fdf as  sdf a   sd  d da       ");
    auto tokens = l.getTokens();

    BOOST_CHECK_EQUAL(tokens.size(), 9);

    auto asdf = tokens[0];
    auto sa = tokens[1];
    auto fdf = tokens[2];
    auto as = tokens[3];
    auto sdf = tokens[4];
    auto a = tokens[5];
    auto sd = tokens[6];
    auto d = tokens[7];
    auto da = tokens[8];
    BOOST_CHECK(asdf.type() == typeid(Identifier));
    BOOST_CHECK(sa.type() == typeid(Identifier));
    BOOST_CHECK(fdf.type() == typeid(Identifier));
    BOOST_CHECK(as.type() == typeid(Identifier));
    BOOST_CHECK(sdf.type() == typeid(Identifier));
    BOOST_CHECK(a.type() == typeid(Identifier));
    BOOST_CHECK(sd.type() == typeid(Identifier));
    BOOST_CHECK(d.type() == typeid(Identifier));
    BOOST_CHECK(da.type() == typeid(Identifier));

    auto asdfId = get<Identifier>(tokens[0]);
    auto saId = get<Identifier>(tokens[1]);
    auto fdfId = get<Identifier>(tokens[2]);
    auto asId = get<Identifier>(tokens[3]);
    auto sdfId = get<Identifier>(tokens[4]);
    auto aId = get<Identifier>(tokens[5]);
    auto sdId = get<Identifier>(tokens[6]);
    auto dId = get<Identifier>(tokens[7]);
    auto daId = get<Identifier>(tokens[8]);

    BOOST_CHECK_EQUAL(asdfId.name, "asdf");
    BOOST_CHECK_EQUAL(saId.name, "sa");
    BOOST_CHECK_EQUAL(fdfId.name, "fdf");
    BOOST_CHECK_EQUAL(asId.name, "as");
    BOOST_CHECK_EQUAL(sdfId.name, "sdf");
    BOOST_CHECK_EQUAL(aId.name, "a");
    BOOST_CHECK_EQUAL(sdId.name, "sd");
    BOOST_CHECK_EQUAL(dId.name, "d");
    BOOST_CHECK_EQUAL(daId.name, "da");
}

BOOST_AUTO_TEST_CASE(SimpleLineBreak)
{
    Lexer l;
    l.tokenize("a\nb");
    auto tokens = l.getTokens();

    BOOST_CHECK_EQUAL(tokens.size(), 3);

    auto a = tokens[0];
    auto n = tokens[1];
    auto b = tokens[2];
    BOOST_CHECK(a.type() == typeid(Identifier));
    BOOST_CHECK(n.type() == typeid(LineBreak));
    BOOST_CHECK(b.type() == typeid(Identifier));
    auto aId = get<Identifier>(tokens[0]);
    auto bId = get<Identifier>(tokens[2]);
    BOOST_CHECK_EQUAL(aId.name, "a");
    BOOST_CHECK_EQUAL(bId.name, "b");
}

BOOST_AUTO_TEST_CASE(DoubleLineBreakNegation)
{
    Lexer l;
    l.tokenize("a\n\nb");
    auto tokens = l.getTokens();

    BOOST_CHECK_EQUAL(tokens.size(), 3);

    auto a = tokens[0];
    auto n = tokens[1];
    auto b = tokens[2];
    BOOST_CHECK(a.type() == typeid(Identifier));
    BOOST_CHECK(n.type() == typeid(LineBreak));
    BOOST_CHECK(b.type() == typeid(Identifier));
    auto aId = get<Identifier>(tokens[0]);
    auto bId = get<Identifier>(tokens[2]);
    BOOST_CHECK_EQUAL(aId.name, "a");
    BOOST_CHECK_EQUAL(bId.name, "b");
}

BOOST_AUTO_TEST_CASE(LineBreakNegationWithIndentation)
{
    Lexer l;
    l.tokenize("a\n    \n\n            \nb");
    auto tokens = l.getTokens();

    BOOST_CHECK_EQUAL(tokens.size(), 3);

    auto a = tokens[0];
    auto n = tokens[1];
    auto b = tokens[2];
    BOOST_CHECK(a.type() == typeid(Identifier));
    BOOST_CHECK(n.type() == typeid(LineBreak));
    BOOST_CHECK(b.type() == typeid(Identifier));
    auto aId = get<Identifier>(tokens[0]);
    auto bId = get<Identifier>(tokens[2]);
    BOOST_CHECK_EQUAL(aId.name, "a");
    BOOST_CHECK_EQUAL(bId.name, "b");
}

BOOST_AUTO_TEST_CASE(XPlusY)
{
    Lexer l;
    l.tokenize("x + y");
    auto tokens = l.getTokens();



    BOOST_CHECK_EQUAL(tokens.size(), 3);

    auto x = tokens[0];
    auto p = tokens[1];
    auto y = tokens[2];
    BOOST_CHECK(x.type() == typeid(Identifier));
    BOOST_CHECK(p.type() == typeid(Identifier));
    BOOST_CHECK(y.type() == typeid(Identifier));
    auto xId = get<Identifier>(tokens[0]);
    auto pId = get<Identifier>(tokens[1]);
    auto yId = get<Identifier>(tokens[2]);
    BOOST_CHECK_EQUAL(xId.name, "x");
    BOOST_CHECK_EQUAL(pId.name, "+");
    BOOST_CHECK_EQUAL(yId.name, "y");
}

BOOST_AUTO_TEST_CASE(XPlusYNoSpaces)
{
    Lexer l;
    l.tokenize("x+y");
    auto tokens = l.getTokens();

    BOOST_CHECK_EQUAL(tokens.size(), 5);

    auto x = tokens[0];
    auto ns1 = tokens[1];
    auto p = tokens[2];
    auto ns2 = tokens[3];
    auto y = tokens[4];
    BOOST_CHECK(x.type() == typeid(Identifier));
    BOOST_CHECK(ns1.type() == typeid(NoSpace));
    BOOST_CHECK(p.type() == typeid(Identifier));
    BOOST_CHECK(ns2.type() == typeid(NoSpace));
    BOOST_CHECK(y.type() == typeid(Identifier));
    auto xId = get<Identifier>(tokens[0]);
    auto pId = get<Identifier>(tokens[2]);
    auto yId = get<Identifier>(tokens[4]);
    BOOST_CHECK_EQUAL(xId.name, "x");
    BOOST_CHECK_EQUAL(pId.name, "+");
    BOOST_CHECK_EQUAL(yId.name, "y");
}

BOOST_AUTO_TEST_CASE(Indentation1)
{
    Lexer l;
    l.tokenize("f\n    x");
    auto tokens = l.getTokens();

    BOOST_CHECK_EQUAL(tokens.size(), 5);

    auto f = tokens[0];
    auto n = tokens[1];
    auto t = tokens[2];
    auto x = tokens[3];
    BOOST_CHECK(f.type() == typeid(Identifier));
    BOOST_CHECK(n.type() == typeid(LineBreak));
    BOOST_CHECK(t.type() == typeid(Tabulation));
    BOOST_CHECK(x.type() == typeid(Identifier));
    auto fId = get<Identifier>(tokens[0]);
    auto xId = get<Identifier>(tokens[3]);
    BOOST_CHECK_EQUAL(fId.name, "f");
    BOOST_CHECK_EQUAL(xId.name, "x");
}

BOOST_AUTO_TEST_CASE(SimpleString)
{
    Lexer l;
    l.tokenize("\"abc\"");
    auto tokens = l.getTokens();

    BOOST_CHECK_EQUAL(tokens.size(), 1);

    auto s = tokens[0];
    BOOST_CHECK(s.type() == typeid(StringData));
    auto st = get<StringData>(tokens[0]);
    BOOST_CHECK_EQUAL(st.data, "abc");
}

BOOST_AUTO_TEST_CASE(SimpleInt)
{
    Lexer l;
    l.tokenize("123");
    auto tokens = l.getTokens();

    BOOST_CHECK_EQUAL(tokens.size(), 1);

    auto s = tokens[0];
    BOOST_CHECK(s.type() == typeid(IntegerData));
    auto st = get<IntegerData>(tokens[0]);
    BOOST_CHECK_EQUAL(st.data, 123);
}

BOOST_AUTO_TEST_CASE(StringApplied)
{
    Lexer l;
    l.tokenize("f \"abc\"");
    auto tokens = l.getTokens();

    BOOST_CHECK_EQUAL(tokens.size(), 2);

    auto ft = tokens[0];
    BOOST_CHECK(ft.type() == typeid(Identifier));
    auto f = get<Identifier>(tokens[0]);
    BOOST_CHECK_EQUAL(f.name, "f");

    auto s = tokens[1];
    BOOST_CHECK(s.type() == typeid(StringData));
    auto st = get<StringData>(tokens[1]);
    BOOST_CHECK_EQUAL(st.data, "abc");
}

BOOST_AUTO_TEST_CASE(IntApplied)
{
    Lexer l;
    l.tokenize("f 0");
    auto tokens = l.getTokens();

    BOOST_CHECK_EQUAL(tokens.size(), 2);

    auto ft = tokens[0];
    BOOST_CHECK(ft.type() == typeid(Identifier));
    auto f = get<Identifier>(tokens[0]);
    BOOST_CHECK_EQUAL(f.name, "f");

    auto s = tokens[1];
    BOOST_CHECK(s.type() == typeid(IntegerData));
    auto st = get<IntegerData>(tokens[1]);
    BOOST_CHECK_EQUAL(st.data, 0);
}

BOOST_AUTO_TEST_CASE(EmptyString)
{
    Lexer l;
    l.tokenize("\"\"");
    auto tokens = l.getTokens();

    BOOST_CHECK_EQUAL(tokens.size(), 1);

    auto s = tokens[0];
    BOOST_CHECK(s.type() == typeid(StringData));
    auto st = get<StringData>(tokens[0]);
    BOOST_CHECK_EQUAL(st.data, "");
}

BOOST_AUTO_TEST_CASE(StringNoSpaces)
{
    Lexer l;
    l.tokenize("f\"abc\"g");
    auto tokens = l.getTokens();

    BOOST_CHECK_EQUAL(tokens.size(), 5);

    auto x = tokens[0];
    auto ns1 = tokens[1];
    auto p = tokens[2];
    auto ns2 = tokens[3];
    auto y = tokens[4];

    BOOST_CHECK(x.type() == typeid(Identifier));
    BOOST_CHECK(ns1.type() == typeid(NoSpace));
    BOOST_CHECK(p.type() == typeid(StringData));
    BOOST_CHECK(ns2.type() == typeid(NoSpace));
    BOOST_CHECK(y.type() == typeid(Identifier));
    auto xId = get<Identifier>(tokens[0]);
    auto pId = get<StringData>(tokens[2]);
    auto yId = get<Identifier>(tokens[4]);
    BOOST_CHECK_EQUAL(xId.name, "f");
    BOOST_CHECK_EQUAL(pId.data, "abc");
    BOOST_CHECK_EQUAL(yId.name, "g");
}

BOOST_AUTO_TEST_CASE(IntNoSpaces)
{
    Lexer l;
    l.tokenize("450g");
    auto tokens = l.getTokens();

    BOOST_CHECK_EQUAL(tokens.size(), 3);

    auto i = tokens[0];
    auto ns1 = tokens[1];
    auto g = tokens[2];

    BOOST_CHECK(i.type() == typeid(IntegerData));
    BOOST_CHECK(ns1.type() == typeid(NoSpace));
    BOOST_CHECK(g.type() == typeid(Identifier));
    auto id = get<Identifier>(tokens[2]);
    auto integer = get<IntegerData>(tokens[0]);
    BOOST_CHECK_EQUAL(id.name, "g");
    BOOST_CHECK_EQUAL(integer.data, 450);
}

BOOST_AUTO_TEST_CASE(Parentheses)
{
    Lexer l;
    l.tokenize("f(x) (a b) c");
    auto tokens = l.getTokens();

    BOOST_CHECK_EQUAL(tokens.size(), 10);

    BOOST_CHECK(tokens[0].type() == typeid(Identifier));
    BOOST_CHECK(tokens[1].type() == typeid(NoSpace));
    BOOST_CHECK(tokens[2].type() == typeid(Opening));
    BOOST_CHECK(tokens[3].type() == typeid(Identifier));
    BOOST_CHECK(tokens[4].type() == typeid(Closing));
    BOOST_CHECK(tokens[5].type() == typeid(Opening));
    BOOST_CHECK(tokens[6].type() == typeid(Identifier));
    BOOST_CHECK(tokens[7].type() == typeid(Identifier));
    BOOST_CHECK(tokens[8].type() == typeid(Closing));
    BOOST_CHECK(tokens[9].type() == typeid(Identifier));
    auto e0 = get<Identifier>(tokens[0]);
    BOOST_CHECK_EQUAL(e0.name, "f");
}

BOOST_AUTO_TEST_CASE(noSpacesNestedFunction)
{
    Lexer l;
    l.tokenize("f(f(f(x)))");
    auto tokens = l.getTokens();

    BOOST_CHECK_EQUAL(tokens.size(), 13);

    BOOST_CHECK(tokens[0].type() == typeid(Identifier));
    BOOST_CHECK(tokens[1].type() == typeid(NoSpace));
    BOOST_CHECK(tokens[2].type() == typeid(Opening));
    BOOST_CHECK(tokens[3].type() == typeid(Identifier));
    BOOST_CHECK(tokens[4].type() == typeid(NoSpace));
    BOOST_CHECK(tokens[5].type() == typeid(Opening));
    BOOST_CHECK(tokens[6].type() == typeid(Identifier));
    BOOST_CHECK(tokens[7].type() == typeid(NoSpace));
    BOOST_CHECK(tokens[8].type() == typeid(Opening));
    BOOST_CHECK(tokens[9].type() == typeid(Identifier));
    BOOST_CHECK(tokens[10].type() == typeid(Closing));
    BOOST_CHECK(tokens[11].type() == typeid(Closing));
    BOOST_CHECK(tokens[12].type() == typeid(Closing));
}
