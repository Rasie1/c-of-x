#define BOOST_TEST_MODULE LanguageTests
#include <boost/test/unit_test.hpp>
#include "Environment.h"
#include "Identifier.h"
#include "Expressions/Any.h"
#include "Void.h"
#include "Parser.h"
#include "Integer.h"
#include "Then.h"

BOOST_AUTO_TEST_CASE(newVariable)
{
    Environment env;
    auto x = env.getEqual(make_ptr<Identifier>("x"));
    BOOST_CHECK(checkType<Any>(x));
}

BOOST_AUTO_TEST_CASE(assignment)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("x = 0", env);
    parsed->eval(env);
    auto x = env.getEqual(make_ptr<Identifier>("x"));
    BOOST_CHECK(checkType<Integer>(x));
    BOOST_CHECK_EQUAL(s_cast<Integer>(x)->value, 0);
}

BOOST_AUTO_TEST_CASE(identityFunction)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("i x = x", env);
    parsed->eval(env);
    auto x = env.getEqual(make_ptr<Identifier>("x"));
    BOOST_CHECK(checkType<Any>(x));
    auto i = env.getEqual(make_ptr<Identifier>("i"));
    auto applied0 = p.parse("i 5", env)->eval(env);
    BOOST_CHECK_EQUAL(d_cast<Integer>(applied0)->value, 5);
}

BOOST_AUTO_TEST_CASE(indirection)
{
    Environment env;
    Parser p;
    p.parse("i x = x", env)->eval(env);
    p.parse("x = 5", env)->eval(env);
    auto parsed = p.parse("i x", env)->eval(env);
    auto result = Identifier::unwrapIfId(parsed, env);
    BOOST_CHECK_EQUAL(d_cast<Integer>(result)->value, 5);
}

BOOST_AUTO_TEST_CASE(twoArgumentFunction)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("plus x y = x + y", env);
    parsed->eval(env);
    auto applied = p.parse("plus 1 2", env)->eval(env);
    BOOST_CHECK_EQUAL(d_cast<Integer>(applied)->value, 3);
}

BOOST_AUTO_TEST_CASE(typedArgument)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("f (int x) = x + 1", env);
    parsed->eval(env);
    auto applied = p.parse("f 0", env)->eval(env);
    BOOST_CHECK_EQUAL(d_cast<Integer>(applied)->value, 1);
}

BOOST_AUTO_TEST_CASE(then)
{
    Environment env;
    Parser p;
    auto then = Then::defaultName;

    auto x = p.parse("0 " + then + " 1", env)->eval(env);
    BOOST_CHECK(checkType<Integer>(x));
    BOOST_CHECK_EQUAL(s_cast<Integer>(x)->value, 1);

    auto y = p.parse("(0 > 1) " + then + " 1", env)->eval(env);
    BOOST_CHECK(checkType<Void>(y));
}

//BOOST_AUTO_TEST_CASE(unionPlus)
//{
//    Environment env;
//    Parser p;
//    auto applied0 = p.parse("(1|2) + 1", env)->eval(env);
//    auto applied1 = p.parse("(1|2) + (10|20)", env)->eval(env);
//    auto applied2 = p.parse("(1|2|3|4) + (100|200|300)", env)->eval(env);

    // not determined in which order they will come, will come up with this
    // test later

//    BOOST_CHECK(d_cast<Operation>(x)->value == 100);

//    p.parse("y > 10", env)->eval(env);
//    auto applied1 = p.parse("y = 0", env)->eval(env);
//    BOOST_CHECK(checkType<Void>(applied1));
//}

BOOST_AUTO_TEST_CASE(moreThan)
{
    Environment env;
    Parser p;
    p.parse("x > 10", env)->eval(env);
    auto applied0 = p.parse("x = 100", env)->eval(env);
    BOOST_CHECK(checkType<Identifier>(applied0));
    auto x = env.getEqual(make_ptr<Identifier>("x"))->eval(env);
    BOOST_CHECK_EQUAL(d_cast<Integer>(x)->value, 100);

    p.parse("y > 10", env)->eval(env);
    auto applied1 = p.parse("y = 0", env)->eval(env);
    BOOST_CHECK(checkType<Void>(applied1));
}

BOOST_AUTO_TEST_CASE(unapplyPlus)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("x + 2 = 0", env);
    parsed->eval(env);
    auto x = env.getEqual(make_ptr<Identifier>("x"));
    BOOST_REQUIRE(checkType<Integer>(x));
    BOOST_CHECK_EQUAL(d_cast<Integer>(x)->value, -2);
}

BOOST_AUTO_TEST_CASE(reverseApplication)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("x = 2 : (+3) : (+4)", env);
    parsed->eval(env);
    auto x = env.getEqual(make_ptr<Identifier>("x"));
    BOOST_CHECK_EQUAL(d_cast<Integer>(x)->value, 9);
}


BOOST_AUTO_TEST_CASE(typedVariables)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("int x = 0", env);
    parsed->eval(env);
    auto x = env.getEqual(make_ptr<Identifier>("x"));
    BOOST_CHECK_EQUAL(d_cast<Integer>(x)->value, 0);

    parsed = p.parse("y : int = 0", env);
    parsed->eval(env);
    auto y = env.getEqual(make_ptr<Identifier>("y"));
    BOOST_CHECK_EQUAL(d_cast<Integer>(y)->value, 0);

    parsed = p.parse("int (x0 = 0)", env);
    parsed->eval(env);
    auto x0 = env.getEqual(make_ptr<Identifier>("x0"));
    BOOST_CHECK_EQUAL(d_cast<Integer>(y)->value, 0);

    parsed = p.parse("(int x1) = 0", env);
    parsed->eval(env);
    auto x1 = env.getEqual(make_ptr<Identifier>("x1"));
    BOOST_CHECK_EQUAL(d_cast<Integer>(y)->value, 0);
}

// recursion test
// re x y = (((x < 11) \ (0)) | ((x > 10) \ (re (x - 1) (print x))) ) x

BOOST_AUTO_TEST_CASE(simpleMacroMatch)
{
    Environment env;
    Parser p;

    auto parsed = p.parse("f ('(1 + 2)) = 4", env);
    parsed->eval(env);
    parsed = p.parse("f (1 + 2)", env);
    auto c = parsed->eval(env);
    BOOST_CHECK_EQUAL(d_cast<Integer>(c)->value, 4);
    parsed = p.parse("f (1 + 3)", env);
    c = parsed->eval(env);
    BOOST_CHECK(checkType<Void>(c));
    parsed = p.parse("f 3", env);
    c = parsed->eval(env);
    BOOST_CHECK(checkType<Void>(c));
}

BOOST_AUTO_TEST_CASE(simpleMacroMatchVariable)
{
    Environment env;
    Parser p;

    auto parsed = p.parse("token ('x) = 0", env);
    parsed->eval(env);
    parsed = p.parse("token c", env);
    auto c = parsed->eval(env);
    BOOST_CHECK_EQUAL(d_cast<Integer>(c)->value, 0);

}

//BOOST_AUTO_TEST_CASE(functionMacroMatch)
//{
//    Environment env;
//    Parser p;

//    p.parse("identity x = x", env)->eval(env);
//    p.parse("token ('(f x)) = f", env)->eval(env);
//    auto parsed = p.parse("token (identity x)", env);
//    auto c = parsed->eval(env);
//    BOOST_CHECK_EQUAL(d_cast<Identifier>(c)->name, "identity");
//}

//BOOST_AUTO_TEST_CASE(plusMacroMatch)
//{
//    Environment env;
//    Parser p;

//    p.parse("plustest ('(x + y)) = x * 10 + y", env)->eval(env);
//    auto parsed = p.parse("plustest(2 + 3)", env);
//    auto c = parsed->eval(env);
//    BOOST_CHECK_EQUAL(d_cast<Integer>(c)->value, 23);
//}

BOOST_AUTO_TEST_CASE(partialPlus)
{
    Environment env;
    Parser p;
    {
        auto parsed = p.parse("(2+) 3", env);
        auto c = parsed->eval(env);
        BOOST_CHECK_EQUAL(d_cast<Integer>(c)->value, 5);
    }
    {
        auto parsed = p.parse("(+3) 2", env);
        auto c = parsed->eval(env);
        BOOST_CHECK_EQUAL(d_cast<Integer>(c)->value, 5);
    }
}

//BOOST_AUTO_TEST_CASE(firstOrderOperator)
//{
//    Environment env;
//    Parser p;
//    {
//        auto parsed = p.parse("(+) 3 2", env);
//        auto c = parsed->eval(env);
//        BOOST_CHECK_EQUAL(d_cast<Integer>(c)->value, 5);
//    }
//    {
//        p.parse("f op x y = op x y", env)->eval(env);
//        auto parsed = p.parse("f (+) 2 3", env);
//        auto c = parsed->eval(env);
//        BOOST_CHECK_EQUAL(d_cast<Integer>(c)->value, 5);
//    }
//}

//BOOST_AUTO_TEST_CASE(functionOverloading)
//{
//    Environment env;
//    Parser p;
//    auto parsed = p.parse("f 1 = 1", env);
//    parsed->eval(env);
//    auto applied0 = p.parse("f 1", env)->eval(env);
//    BOOST_CHECK_EQUAL(d_cast<Integer>(applied0)->value, 1);
//    auto applied1 = p.parse("f 2", env)->eval(env);
//    BOOST_CHECK(d_cast<Void>(applied1));
//    parsed = p.parse("f 2 = 2", env);
//    parsed->eval(env);
//    auto applied2 = p.parse("f 1", env)->eval(env);
//    BOOST_CHECK_EQUAL(d_cast<Integer>(applied2)->value, 1);
//    auto applied3 = p.parse("f 2", env)->eval(env);
//    BOOST_CHECK_EQUAL(d_cast<Integer>(applied3)->value, 2);
//    auto applied4 = p.parse("f 3", env)->eval(env);
//    BOOST_CHECK(d_cast<Void>(applied4));
//}
