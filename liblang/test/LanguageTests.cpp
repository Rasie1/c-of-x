#define BOOST_TEST_MODULE LanguageTests
#include <boost/test/unit_test.hpp>
#include "System/Environment.h"
#include "Expressions/Identifier.h"
#include "Expressions/Any.h"
#include "Expressions/Void.h"
#include "Parser/Parser.h"
#include "Expressions/Integer.h"
#include "Expressions/ValueInSet.h"
#include "Expressions/IntegerType.h"
#include "Expressions/Then.h"
#include "Expressions/More.h"
#include "Expressions/Less.h"
#include "System/Cast.h"

BOOST_AUTO_TEST_CASE(newVariable)
{
    Environment env;
    auto x = env.getEqual(makeObject<Identifier>("x"));
    BOOST_REQUIRE(checkType<Any>(x));
}

BOOST_AUTO_TEST_CASE(assignment)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("x = 0", env);
    parsed->eval(env);
    auto x = env.getEqual(makeObject<Identifier>("x"));
    BOOST_REQUIRE(checkType<Integer>(x));
    BOOST_CHECK_EQUAL(d_cast<Integer>(x)->value, 0);
}

BOOST_AUTO_TEST_CASE(identityFunction)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("i x = x", env);
    parsed->eval(env);
    auto x = env.getEqual(makeObject<Identifier>("x"));
    BOOST_REQUIRE(checkType<Any>(x));
    auto i = env.getEqual(makeObject<Identifier>("i"));
    auto applied0 = p.parse("i 5", env)->eval(env);
    BOOST_CHECK_EQUAL(d_cast<Integer>(applied0)->value, 5);
}

BOOST_AUTO_TEST_CASE(identityType)
{
    Environment env;
    Parser p;
    p.parse("i x = x", env)->eval(env);
    p.parse("a = 5",   env)->eval(env);
    auto a = p.parse("i a", env)->eval(env);
    BOOST_REQUIRE(checkType<Identifier>(a));
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
    BOOST_REQUIRE(checkType<Integer>(applied));
    BOOST_CHECK_EQUAL(d_cast<Integer>(applied)->value, 1);
}

BOOST_AUTO_TEST_CASE(then)
{
    Environment env;
    Parser p;
    auto then = Then::defaultName;

    auto x = p.parse("0 " + then + " 1", env)->eval(env);
    BOOST_REQUIRE(checkType<Integer>(x));
    BOOST_CHECK_EQUAL(d_cast<Integer>(x)->value, 1);

    auto y = p.parse("(0 > 1) " + then + " 1", env)->eval(env);
    BOOST_REQUIRE(checkType<Void>(y));
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
//    BOOST_REQUIRE(checkType<Void>(applied1));
//}

BOOST_AUTO_TEST_CASE(moreThan)
{
    Environment env;
    Parser p;
    p.parse("x > 10", env)->eval(env);
    auto applied0 = p.parse("x = 100", env)->eval(env);
    BOOST_REQUIRE(checkType<Identifier>(applied0));
    auto x = env.getEqual(makeObject<Identifier>("x"))->eval(env);
    BOOST_CHECK_EQUAL(d_cast<Integer>(x)->value, 100);

    p.parse("y > 10", env)->eval(env);
    auto applied1 = p.parse("y = 0", env)->eval(env);
    BOOST_REQUIRE(checkType<Identifier>(applied1));
    auto y = env.getEqual(applied1);
    BOOST_REQUIRE(checkType<Void>(y));
}

BOOST_AUTO_TEST_CASE(DoubleNegative)
{
    Environment env;
    Parser p;
    {
        auto x = p.parse("!(!(<3))", env)->eval(env);
        BOOST_REQUIRE(checkType<LessThan>(x));
    }
    {
        auto x = p.parse("!(!(>3))", env)->eval(env);
        BOOST_REQUIRE(checkType<MoreThan>(x));
    }
}

BOOST_AUTO_TEST_CASE(substitutionInPartialApplication)
{
    Environment env;
    Parser p;
    p.parse("arg = 3", env)->eval(env);
    auto applied0 = p.parse("x = (+1) arg", env)->eval(env);
    BOOST_REQUIRE(checkType<Identifier>(applied0));
    auto x = env.getEqual(makeObject<Identifier>("x"))->eval(env);
    BOOST_REQUIRE(checkType<Integer>(x));
    BOOST_CHECK_EQUAL(d_cast<Integer>(x)->value, 4);
}

BOOST_AUTO_TEST_CASE(unapplyPlus)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("x + 2 = 0", env);
    parsed->eval(env);
    auto x = env.getEqual(makeObject<Identifier>("x"));
    BOOST_REQUIRE(checkType<Integer>(x));
    BOOST_CHECK_EQUAL(d_cast<Integer>(x)->value, -2);
}

BOOST_AUTO_TEST_CASE(unapplyPlusAsArgument)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("f (x + 2) = x", env);
    parsed->eval(env);
    parsed = p.parse("f 0", env);
    auto x = parsed->eval(env);
    BOOST_REQUIRE(checkType<Integer>(x));
    BOOST_CHECK_EQUAL(d_cast<Integer>(x)->value, -2);
}

BOOST_AUTO_TEST_CASE(unapplyPartialPlus)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("(+2) x = 0", env);
    parsed->eval(env);
    auto x = env.getEqual(makeObject<Identifier>("x"));
    BOOST_REQUIRE(checkType<Integer>(x));
    BOOST_CHECK_EQUAL(d_cast<Integer>(x)->value, -2);
}

BOOST_AUTO_TEST_CASE(unapplyPartialPlusAsArgument)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("f ((+2) x) = x", env);
    parsed->eval(env);
    parsed = p.parse("f 0", env);
    auto x = parsed->eval(env);
    BOOST_REQUIRE(checkType<Integer>(x));
    BOOST_CHECK_EQUAL(d_cast<Integer>(x)->value, -2);
}

BOOST_AUTO_TEST_CASE(reverseApplication)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("x = 2 : (+3) : (+4)", env);
    parsed->eval(env);
    auto x = env.getEqual(makeObject<Identifier>("x"));
    BOOST_CHECK_EQUAL(d_cast<Integer>(x)->value, 9);
}


BOOST_AUTO_TEST_CASE(typedVariables)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("int x = 0", env);
    parsed->eval(env);
    auto x = env.getEqual(makeObject<Identifier>("x"));
    BOOST_REQUIRE(checkType<Integer>(x));
    BOOST_CHECK_EQUAL(d_cast<Integer>(x)->value, 0);

    parsed = p.parse("y : int = 0", env);
    parsed->eval(env);
    auto y = env.getEqual(makeObject<Identifier>("y"));
    BOOST_REQUIRE(checkType<Integer>(y));
    BOOST_CHECK_EQUAL(d_cast<Integer>(y)->value, 0);

    parsed = p.parse("int (x0 = 0)", env);
    parsed->eval(env);
    auto x0 = env.getEqual(makeObject<Identifier>("x0"));
    BOOST_REQUIRE(checkType<Integer>(x0));
    BOOST_CHECK_EQUAL(d_cast<Integer>(x0)->value, 0);

    parsed = p.parse("(int x1) = 0", env);
    parsed->eval(env);
    auto x1 = env.getEqual(makeObject<Identifier>("x1"));
    BOOST_REQUIRE(checkType<Integer>(x1));
    BOOST_CHECK_EQUAL(d_cast<Integer>(x1)->value, 0);
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
    BOOST_REQUIRE(checkType<Void>(c));
    parsed = p.parse("f 3", env);
    c = parsed->eval(env);
    BOOST_REQUIRE(checkType<Void>(c));
}

// TODO: why did I wrote this test?
// BOOST_AUTO_TEST_CASE(simpleMacroMatchVariable)
// {
//     Environment env;
//     Parser p;

//     auto parsed = p.parse("token ('x) = 0", env);
//     parsed->eval(env);
//     parsed = p.parse("token c", env);
//     auto c = parsed->eval(env);
//     BOOST_CHECK_EQUAL(d_cast<Integer>(c)->value, 0);

// }

// BOOST_AUTO_TEST_CASE(functionMacroMatch)
// {
//     Environment env;
//     Parser p;
 
//     p.parse("identity x = x", env)->eval(env);
//     p.parse("token ('(f x)) = f", env)->eval(env);
//     auto parsed = p.parse("token (identity x)", env);
//     auto c = parsed->eval(env);
//     BOOST_CHECK_EQUAL(d_cast<Identifier>(c)->name, "identity");
// }

// BOOST_AUTO_TEST_CASE(plusMacroMatch)
// {
//     Environment env;
//     Parser p;
 
//     p.parse("plustest ('(x + y)) = x * 10 + y", env)->eval(env);
//     auto parsed = p.parse("plustest(2 + 3)", env);
//     auto c = parsed->eval(env);
//     BOOST_CHECK_EQUAL(d_cast<Integer>(c)->value, 23);
// }

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

BOOST_AUTO_TEST_CASE(operatorAsNonInfix)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("(+) 3 2", env);
    auto c = parsed->eval(env);
    BOOST_CHECK_EQUAL(d_cast<Integer>(c)->value, 5);
}

BOOST_AUTO_TEST_CASE(firstOrderOperator)
{
    Environment env;
    Parser p;
    p.parse("f op x y = op x y", env)->eval(env);
    auto parsed = p.parse("f (+) 2 3", env);
    auto c = parsed->eval(env);
    BOOST_CHECK_EQUAL(d_cast<Integer>(c)->value, 5);
}

BOOST_AUTO_TEST_CASE(firstOrderAsNonInfix)
{
    return; //unimplemented
    Environment env;
    Parser p;
    {
        auto parsed = p.parse("(=) x 0", env);
        auto c = parsed->eval(env);
        // BOOST_CHECK_EQUAL(d_cast<Integer>(c)->value, 5);
        auto x = env.getEqual(makeObject<Identifier>("x"))->eval(env);
        BOOST_CHECK_EQUAL(d_cast<Integer>(x)->value, 0);
    }
    {
        auto parsed = p.parse("(=) y 0", env);
        auto c = parsed->eval(env);
        // BOOST_CHECK_EQUAL(d_cast<Integer>(c)->value, 5);
        auto x = env.getEqual(makeObject<Identifier>("y"))->eval(env);
        BOOST_CHECK_EQUAL(d_cast<Integer>(x)->value, 0);
    }
}

BOOST_AUTO_TEST_CASE(functionOverloading)
{
    return; // unimplemented
    Environment env;
    Parser p;
    auto parsed = p.parse("f 1 = 1", env);
    parsed->eval(env);
    auto applied0 = p.parse("f 1", env)->eval(env);
    BOOST_CHECK_EQUAL(d_cast<Integer>(applied0)->value, 1);
    auto applied1 = p.parse("f 2", env)->eval(env);
    BOOST_CHECK(d_cast<Void>(applied1));
    parsed = p.parse("f 2 = 2", env);
    parsed->eval(env);
    auto applied2 = p.parse("f 1", env)->eval(env);
    BOOST_CHECK_EQUAL(d_cast<Integer>(applied2)->value, 1);
    auto applied3 = p.parse("f 2", env)->eval(env);
    BOOST_CHECK_EQUAL(d_cast<Integer>(applied3)->value, 2);
    auto applied4 = p.parse("f 3", env)->eval(env);
    BOOST_CHECK(d_cast<Void>(applied4));
}

BOOST_AUTO_TEST_CASE(unionAsFunctionArgumentSimple)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("f (1 | 2) = 3", env);
    parsed->eval(env);
    auto applied0 = p.parse("f 1", env)->eval(env);
    BOOST_CHECK_EQUAL(d_cast<Integer>(applied0)->value, 3);
    auto applied1 = p.parse("f 2", env)->eval(env);
    BOOST_CHECK_EQUAL(d_cast<Integer>(applied1)->value, 3);
    auto applied2 = p.parse("f 3", env)->eval(env);
    BOOST_REQUIRE(checkType<Void>(applied2));
}

BOOST_AUTO_TEST_CASE(simpleComplementAsArgument)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("f (!1) = 3", env);
    parsed->eval(env);
    auto applied0 = p.parse("f 2", env)->eval(env);
    BOOST_CHECK_EQUAL(d_cast<Integer>(applied0)->value, 3);
    auto applied2 = p.parse("f 1", env)->eval(env);
    BOOST_REQUIRE(checkType<Void>(applied2));
}

BOOST_AUTO_TEST_CASE(simpleInverseFunction)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("f (int x) = x + 1", env);
    parsed->eval(env);
    auto applied = p.parse("(inverse f) 0", env)->eval(env);
    BOOST_CHECK_EQUAL(d_cast<Integer>(applied)->value, -1);
}

BOOST_AUTO_TEST_CASE(evaluatingInverseFunction)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("f x = x + 10", env);
    parsed->eval(env);
    {
        parsed = p.parse("m (int x) = f 10 + f 17", env);
        parsed->eval(env);
        auto applied = p.parse("(inverse m) 47", env)->eval(env);
        BOOST_REQUIRE(checkType<ValueInSet>(applied));
        applied = p.parse("(inverse m) 46", env)->eval(env);
        BOOST_REQUIRE(checkType<Void>(applied));
    }
    {
        parsed = p.parse("n 0 = f 10 + f 17", env);
        parsed->eval(env);
        auto applied = p.parse("(inverse n) 47", env)->eval(env);
        BOOST_CHECK_EQUAL(d_cast<Integer>(applied)->value, 0);
        applied = p.parse("(inverse n) 46", env)->eval(env);
        BOOST_REQUIRE(checkType<Void>(applied));
    }
}

BOOST_AUTO_TEST_CASE(simpleClosure)
{
    Environment env;
    Parser p;
    {
        auto parsed = p.parse("(x => x) 3", env);
        auto c = parsed->eval(env);
        BOOST_CHECK_EQUAL(d_cast<Integer>(c)->value, 3);
    }
    {
        auto parsed = p.parse("(3 => 3) 3", env);
        auto c = parsed->eval(env);
        BOOST_CHECK_EQUAL(d_cast<Integer>(c)->value, 3);
    }
    {
        auto parsed = p.parse("(2 => 3) 3", env);
        auto c = parsed->eval(env);
        BOOST_REQUIRE(checkType<Void>(c));
    }
}

BOOST_AUTO_TEST_CASE(complexClosure)
{
    Environment env;
    Parser p;
    {
        auto parsed = p.parse("(int x => x) 3", env);
        auto c = parsed->eval(env);
        BOOST_CHECK_EQUAL(d_cast<Integer>(c)->value, 3);
    }
    {
        auto parsed = p.parse("(2|3 => 3) 3", env);
        auto c = parsed->eval(env);
        BOOST_CHECK_EQUAL(d_cast<Integer>(c)->value, 3);
    }
    {
        auto parsed = p.parse("(1|2 => 3) 3", env);
        auto c = parsed->eval(env);
        BOOST_REQUIRE(checkType<Void>(c));
    }
    {
        auto parsed = p.parse("(int x => 3) \"abc\"", env);
        auto c = parsed->eval(env);
        BOOST_REQUIRE(checkType<Void>(c));
    }
}

BOOST_AUTO_TEST_CASE(closurePredicate)
{
    {
        Environment env;
        Parser p;
        auto parsed = p.parse("x : (x => (x = 0))", env);
        parsed->eval(env);
        parsed = p.parse("x = 0", env);
        parsed->eval(env);
        auto x = env.getEqual(makeObject<Identifier>("x"));
        BOOST_REQUIRE(checkType<Integer>(x));
        BOOST_CHECK_EQUAL(d_cast<Integer>(x)->value, 0);
    }
    {
        Environment env;
        Parser p;
        auto parsed = p.parse("x : (x => (x = 0))", env);
        parsed->eval(env);
        parsed = p.parse("x = 1", env);
        parsed->eval(env);
        auto x = env.getEqual(makeObject<Identifier>("x"));
        BOOST_REQUIRE(checkType<Void>(x));
    }
}

BOOST_AUTO_TEST_CASE(intersectionMorphism)
{
    Environment env;
    Parser p;
    {
        auto x = p.parse("(>0) 0", env)->eval(env);
        BOOST_REQUIRE(checkType<Void>(x));
    }
    {
        auto x = p.parse("(>0) 1", env)->eval(env);
        BOOST_CHECK_EQUAL(d_cast<Integer>(x)->value, 1);
    }
    {
        auto x = p.parse("((>0) & (>0)) 0", env)->eval(env);
        BOOST_REQUIRE(checkType<Void>(x));
    }
    {
        auto x = p.parse("((>0) & (>0)) 1", env)->eval(env);
        BOOST_CHECK_EQUAL(d_cast<Integer>(x)->value, 1);
    }
    {
        auto x = p.parse("((>0) & (<10)) 1", env)->eval(env);
        BOOST_CHECK_EQUAL(d_cast<Integer>(x)->value, 1);
    }
    {
        auto x = p.parse("((>0) & (<10)) 11", env)->eval(env);
        BOOST_REQUIRE(checkType<Void>(x));
    }
}

BOOST_AUTO_TEST_CASE(closureIntersection)
{
   Environment env;
   Parser p;
   {
       auto parsed = p.parse("((1 => 1) & (1 => 1)) 1", env);
       auto c = parsed->eval(env);
       BOOST_CHECK_EQUAL(d_cast<Integer>(c)->value, 1);
   }
   {
       auto parsed = p.parse("((int x => 1) & (1 => 1)) 1", env);
       auto c = parsed->eval(env);
       BOOST_CHECK_EQUAL(d_cast<Integer>(c)->value, 1);
   }
   {
       auto parsed = p.parse("((int x => 1) & (2 => 1)) 1", env);
       auto c = parsed->eval(env);
       BOOST_REQUIRE(checkType<Void>(c));
   }
   {
       auto parsed = p.parse("((int x => 1) & (1 => 1)) 2", env);
       auto c = parsed->eval(env);
       BOOST_REQUIRE(checkType<Void>(c));
   }
}

// BOOST_AUTO_TEST_CASE(intersectionType)
// {
//     {
//         Environment env;
//         Parser p;
//         auto parsed = p.parse("x : ((>0) & (<10))", env);
//         parsed->eval(env);
//         parsed = p.parse("x = 5", env);
//         parsed->eval(env);
//         auto x = env.getEqual(makeObject<Identifier>("x"));
//         BOOST_REQUIRE(checkType<Integer>(x));
//         BOOST_CHECK_EQUAL(d_cast<Integer>(x)->value, 5);
//     }
//     {
//         Environment env;
//         Parser p;
//         auto parsed = p.parse("x : ((>0) & (<10))", env);
//         parsed->eval(env);
//         parsed = p.parse("x = 0", env);
//         parsed->eval(env);
//         auto x = env.getEqual(makeObject<Identifier>("x"));
//         BOOST_REQUIRE(checkType<Void>(x));
//     }
//     {
//         Environment env;
//         Parser p;
//         auto parsed = p.parse("x : ((>0) & (<10))", env);
//         parsed->eval(env);
//         parsed = p.parse("x = 10", env);
//         parsed->eval(env);
//         auto x = env.getEqual(makeObject<Identifier>("x"));
//         BOOST_REQUIRE(checkType<Void>(x));
//     }
// }


// BOOST_AUTO_TEST_CASE(unionType)
// {
//     {
//         Environment env;
//         Parser p;
//         auto parsed = p.parse("x : ((=0) | (=5))", env);
//         parsed->eval(env);
//         parsed = p.parse("x = 5", env);
//         parsed->eval(env);
//         auto x = env.getEqual(makeObject<Identifier>("x"));
//         BOOST_REQUIRE(checkType<Integer>(x));
//         BOOST_CHECK_EQUAL(d_cast<Integer>(x)->value, 5);
//     }
//     {
//         Environment env;
//         Parser p;
//         auto parsed = p.parse("x : ((=0) | (=5))", env);
//         parsed->eval(env);
//         parsed = p.parse("x = 0", env);
//         parsed->eval(env);
//         auto x = env.getEqual(makeObject<Identifier>("x"));
//         BOOST_REQUIRE(checkType<Integer>(x));
//         BOOST_CHECK_EQUAL(d_cast<Integer>(x)->value, 0);
//     }
//     {
//         Environment env;
//         Parser p;
//         auto parsed = p.parse("x : ((=1) | (=2))", env);
//         parsed->eval(env);
//         parsed = p.parse("x = 0", env);
//         parsed->eval(env);
//         auto x = env.getEqual(makeObject<Identifier>("x"));
//         BOOST_REQUIRE(checkType<Void>(x));
//     }
//     {
//         Environment env;
//         Parser p;
//         auto parsed = p.parse("x : ((=9) | (=11))", env);
//         parsed->eval(env);
//         parsed = p.parse("x = 10", env);
//         parsed->eval(env);
//         auto x = env.getEqual(makeObject<Identifier>("x"));
//         BOOST_REQUIRE(checkType<Void>(x));
//     }
// }

// BOOST_AUTO_TEST_CASE(closureIntersectionType)
// {
//     {
//         Environment env;
//         Parser p;
//         auto parsed = p.parse("x : ((x => (x > 0)) & (x => (x < 10)))", env);
//         parsed->eval(env);
//         parsed = p.parse("x = 5", env);
//         parsed->eval(env);
//         auto x = env.getEqual(makeObject<Identifier>("x"));
//         BOOST_REQUIRE(checkType<Integer>(x));
//         BOOST_CHECK_EQUAL(d_cast<Integer>(x)->value, 5);
//     }
//     {
//         Environment env;
//         Parser p;
//         auto parsed = p.parse("x : ((x => (x > 0)) & (x => (x < 10)))", env);
//         parsed->eval(env);
//         parsed = p.parse("x = 0", env);
//         parsed->eval(env);
//         auto x = env.getEqual(makeObject<Identifier>("x"));
//         BOOST_REQUIRE(checkType<Void>(x));
//     }
//     {
//         Environment env;
//         Parser p;
//         auto parsed = p.parse("x : ((x => int x) & (>0))", env);
//         parsed->eval(env);
//         parsed = p.parse("x = 0", env);
//         parsed->eval(env);
//         auto x = env.getEqual(makeObject<Identifier>("x"));
//         BOOST_REQUIRE(checkType<Void>(x));
//     }
// }

BOOST_AUTO_TEST_CASE(noEnvironmentSharing)
{
    Environment env;
    Parser p;
    {
        auto parsed = p.parse("y = 1", env);
        parsed->eval(env);
        parsed = p.parse("x = ((+1) y) + ((+1) y)", env);
        parsed->eval(env);
        auto x = env.getEqual(makeObject<Identifier>("x"));
        BOOST_REQUIRE(checkType<Integer>(x));
        BOOST_CHECK_EQUAL(d_cast<Integer>(x)->value, 4);
        auto y = env.getEqual(makeObject<Identifier>("y"));
        BOOST_REQUIRE(checkType<Integer>(y));
        BOOST_CHECK_EQUAL(d_cast<Integer>(y)->value, 1);
    }
    {
        auto parsed = p.parse("z = 2", env);
        parsed->eval(env);
        parsed = p.parse("w = (int z) + (int z)", env);
        parsed->eval(env);
        auto x = env.getEqual(makeObject<Identifier>("w"));
        BOOST_REQUIRE(checkType<Integer>(x));
        BOOST_CHECK_EQUAL(d_cast<Integer>(x)->value, 4);
    }
}

BOOST_AUTO_TEST_CASE(simpleLet)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("f x = let (a = x + 2) a", env);
    parsed->eval(env);
    parsed = p.parse("z = f 1", env);
    parsed->eval(env);
    auto x = env.getEqual(makeObject<Identifier>("z"));
    BOOST_REQUIRE(checkType<Integer>(x));
    BOOST_CHECK_EQUAL(d_cast<Integer>(x)->value, 3);
}

BOOST_AUTO_TEST_CASE(letSharing)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("let (x = 3)", env);
    parsed->eval(env);
    auto x = env.getEqual(makeObject<Identifier>("x"));
    BOOST_REQUIRE(checkType<Integer>(x));
    BOOST_CHECK_EQUAL(d_cast<Integer>(x)->value, 3);
}

BOOST_AUTO_TEST_CASE(contains)
{
    {
        Environment env;
        Parser p;
        auto parsed = p.parse("x : (=(1 | 2))", env);
        parsed->eval(env);
        parsed = p.parse("x = 1", env);
        parsed->eval(env);
        auto x = env.getEqual(makeObject<Identifier>("x"));
        BOOST_REQUIRE(checkType<Integer>(x));
        BOOST_CHECK_EQUAL(d_cast<Integer>(x)->value, 1);
    }
    {
        Environment env;
        Parser p;
        auto parsed = p.parse("x : (=(1 | 2 | (2 + 1) | (2 + 2) | 5))", env);
        parsed->eval(env);
        parsed = p.parse("x = 3", env);
        parsed->eval(env);
        auto x = env.getEqual(makeObject<Identifier>("x"));
        BOOST_REQUIRE(checkType<Integer>(x));
        BOOST_CHECK_EQUAL(d_cast<Integer>(x)->value, 3);
    }
}
