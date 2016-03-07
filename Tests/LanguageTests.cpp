#define BOOST_TEST_MODULE LanguageTests
#include <boost/test/unit_test.hpp>
#include "Environment.h"
#include "Identifier.h"
#include "Any.h"
#include "Void.h"
#include "Parser.h"
#include "Integer.h"

BOOST_AUTO_TEST_CASE(newVariable)
{
    Environment env;
    auto x = env.getEqual(make_ptr<Identifier>("x"));
    BOOST_CHECK(checkType<Any>(x));
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
    BOOST_CHECK(d_cast<Integer>(applied0)->value == 5);
    auto applied1 = p.parse("i x", env)->eval(env);
    BOOST_CHECK(applied1->show() == "x");
}

BOOST_AUTO_TEST_CASE(twoArgumentFunction)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("plus  x y = x + y", env);
    parsed->eval(env);
    auto applied = p.parse("plus 1 2", env)->eval(env);
    BOOST_CHECK(d_cast<Integer>(applied)->value == 3);
}

BOOST_AUTO_TEST_CASE(typedArgument)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("f (int x) = x + 1", env);
    parsed->eval(env);
    auto applied = p.parse("f 0", env)->eval(env);
    BOOST_CHECK(d_cast<Integer>(applied)->value == 1);
}

BOOST_AUTO_TEST_CASE(unapplyPlus)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("x + 2 = 0", env);
    parsed->eval(env);
    auto x = env.getEqual(make_ptr<Identifier>("x"));
    BOOST_CHECK(d_cast<Integer>(x)->value == -2);
}

BOOST_AUTO_TEST_CASE(moreThan)
{
    Environment env;
    Parser p;
    p.parse("x > 10", env)->eval(env);
    auto applied0 = p.parse("x = 100", env)->eval(env);
    BOOST_CHECK(checkType<Identifier>(applied0));
    auto x = env.getEqual(make_ptr<Identifier>("x"));
    BOOST_CHECK(d_cast<Integer>(x)->value == 100);

    p.parse("y > 10", env)->eval(env);
    auto applied1 = p.parse("y = 0", env)->eval(env);
    BOOST_CHECK(checkType<Void>(applied1));
}

