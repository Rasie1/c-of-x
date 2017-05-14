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



auto execute(Environment& env, const Object& e)
{
    // probably not correct
    e->eval(env)->eval(env)->eval(env)->eval(env);
}

BOOST_AUTO_TEST_CASE(castIdentifier)
{
    Environment env;
    Parser p;
    auto x = p.parse("x", env);
    BOOST_CHECK(checkType<Identifier>(env, x));
    auto o = p.parse("0", env);
    BOOST_CHECK(!checkType<Identifier>(env, o));
}

BOOST_AUTO_TEST_CASE(castSearch)
{
    Environment env;
    Parser p;
    execute(env, p.parse("a = 0", env));
    execute(env, p.parse("a = 0", env));
    auto a = env.getEqual("a");

    BOOST_REQUIRE(checkType<Integer>(env, a));
    BOOST_CHECK_EQUAL(cast<Integer>(env, a)->value, 0);
}

BOOST_AUTO_TEST_CASE(newVariable)
{
    Environment env;
    auto x = env.getEqual("x");
    BOOST_REQUIRE(checkType<Any>(env, x));
}

BOOST_AUTO_TEST_CASE(assignment)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("x = 0", env);
    execute(env, parsed);//
    auto x = env.getEqual("x");
    BOOST_REQUIRE(checkType<Integer>(env, x));
    BOOST_CHECK_EQUAL(cast<Integer>(env, x)->value, 0);
}

BOOST_AUTO_TEST_CASE(identityFunction)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("i x = x", env);
    execute(env, parsed);//
    auto x = env.getEqual("x");
    BOOST_REQUIRE(checkType<Any>(env, x));
    auto i = env.getEqual("i");
    auto applied0 = p.parse("i 5", env);
    BOOST_CHECK_EQUAL(cast<Integer>(env, applied0)->value, 5);
}

BOOST_AUTO_TEST_CASE(identityType)
{
    Environment env;
    Parser p;
    execute(env, p.parse("i x = x", env));
    execute(env, p.parse("a = 5",   env));
    execute(env, p.parse("i a",   env));
    auto a = p.parse("a", env);
    BOOST_REQUIRE(checkType<Identifier>(env, a));
    BOOST_CHECK_EQUAL(cast<Integer>(env, a)->value, 5);
}

BOOST_AUTO_TEST_CASE(indirection)
{
    Environment env;
    Parser p;
    execute(env, p.parse("i x = x", env));
    execute(env, p.parse("x = 5", env));
    execute(env, p.parse("i x", env));
    auto parsed = p.parse("x", env);
    auto result = Identifier::unwrapIfId(parsed, env);
    BOOST_CHECK_EQUAL(cast<Integer>(env, result)->value, 5);
}

BOOST_AUTO_TEST_CASE(twoArgumentFunction)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("plus x y = x + y", env);
    execute(env, parsed);//
    auto applied = p.parse("plus 1 2", env);
    execute(env, applied);//
    BOOST_CHECK_EQUAL(cast<Integer>(env, applied)->value, 3);
}

BOOST_AUTO_TEST_CASE(typedArgument)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("f (int x) = x + 1", env);
    execute(env, parsed);//
    auto applied = p.parse("f 0", env);
    execute(env, applied);//
    BOOST_REQUIRE(checkType<Integer>(env, applied));
    BOOST_CHECK_EQUAL(cast<Integer>(env, applied)->value, 1);
}

BOOST_AUTO_TEST_CASE(then)
{
    Environment env;
    Parser p;
    auto then = Then::defaultName;

    auto x = p.parse("0 " + then + " 1", env);
    BOOST_REQUIRE(checkType<Integer>(env, x));
    BOOST_CHECK_EQUAL(cast<Integer>(env, x)->value, 1);

    auto y = p.parse("(0 > 1) " + then + " 1", env);
    BOOST_REQUIRE(checkType<Void>(env, y));
}

//BOOST_AUTO_TEST_CASE(unionPlus)
//{
//    Environment env;
//    Parser p;
//    auto applied0 = p.parse("(1|2) + 1", env);
//    auto applied1 = p.parse("(1|2) + (10|20)", env);
//    auto applied2 = p.parse("(1|2|3|4) + (100|200|300)", env);

    // not determined in which order they will come, will come up with this
    // test later

//    BOOST_CHECK(cast<Operation>(env, x)->value == 100);

//    p.parse("y > 10", env);
//    auto applied1 = p.parse("y = 0", env);
//    BOOST_REQUIRE(checkType<Void>(env, applied1));
//}

BOOST_AUTO_TEST_CASE(contradiction)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("y = 0", env);
    execute(env, parsed);
    parsed = p.parse("y = 1", env);
    execute(env, parsed);
    auto y = makeObject<Identifier>("y");
    BOOST_REQUIRE(checkType<Void>(env, y));
}

BOOST_AUTO_TEST_CASE(moreThan)
{
    Environment env;
    Parser p;
    {
        auto parsed = p.parse("x > 10", env);
        execute(env, parsed);
        parsed = p.parse("x = 100", env);
        execute(env, parsed);
        auto x = makeObject<Identifier>("x");
        BOOST_CHECK_EQUAL(cast<Integer>(env, x)->value, 100);
    }
    {
        auto parsed = p.parse("y > 10", env);
        execute(env, parsed);
        parsed = p.parse("y = 0", env);
        execute(env, parsed);
        auto y = makeObject<Identifier>("y");
        BOOST_REQUIRE(checkType<Void>(env, y));
    }
}

BOOST_AUTO_TEST_CASE(doubleNegative)
{
    Environment env;
    Parser p;
    {
        auto x = p.parse("!(!(<3))", env);
        BOOST_REQUIRE(checkType<LessThan>(env, x));
    }
    {
        auto x = p.parse("!(!(>3))", env);
        BOOST_REQUIRE(checkType<MoreThan>(env, x));
    }
}

BOOST_AUTO_TEST_CASE(substitutionInPartialApplication)
{
    Environment env;
    Parser p;
    auto arg = p.parse("arg = 3", env);
    execute(env, arg);
    auto applied0 = p.parse("x = (+1) arg", env);
    execute(env, applied0);
    BOOST_REQUIRE(checkType<Identifier>(env, applied0));
    auto x = env.getEqual("x");
    BOOST_REQUIRE(checkType<Integer>(env, x));
    BOOST_CHECK_EQUAL(cast<Integer>(env, x)->value, 4);
}

BOOST_AUTO_TEST_CASE(unapplyPlus)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("x + 2 = 0", env);
    execute(env, parsed);//
    auto x = env.getEqual("x");
    BOOST_REQUIRE(checkType<Integer>(env, x));
    BOOST_CHECK_EQUAL(cast<Integer>(env, x)->value, -2);
}

BOOST_AUTO_TEST_CASE(unapplyPlusAsArgument)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("f (x + 2) = x", env);
    execute(env, parsed);//
    parsed = p.parse("f 0", env);
    auto x = parsed;
    BOOST_REQUIRE(checkType<Integer>(env, x));
    BOOST_CHECK_EQUAL(cast<Integer>(env, x)->value, -2);
}

BOOST_AUTO_TEST_CASE(unapplyPartialPlus)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("(+2) x = 0", env);
    execute(env, parsed);//
    auto x = env.getEqual("x");
    BOOST_REQUIRE(checkType<Integer>(env, x));
    BOOST_CHECK_EQUAL(cast<Integer>(env, x)->value, -2);
}

BOOST_AUTO_TEST_CASE(unapplyPartialPlusAsArgument)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("f ((+2) x) = x", env);
    execute(env, parsed);//
    parsed = p.parse("f 0", env);
    auto x = parsed;
    BOOST_REQUIRE(checkType<Integer>(env, x));
    BOOST_CHECK_EQUAL(cast<Integer>(env, x)->value, -2);
}

BOOST_AUTO_TEST_CASE(reverseApplication)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("x = 2 : (+3) : (+4)", env);
    execute(env, parsed);//
    auto x = env.getEqual("x");
    BOOST_CHECK_EQUAL(cast<Integer>(env, x)->value, 9);
}


BOOST_AUTO_TEST_CASE(typedVariables0)
{
    Environment env;
    Parser p;

    auto parsed = p.parse("int x = 0", env);
    execute(env, parsed);//
    auto x = env.getEqual("x");
    BOOST_REQUIRE(checkType<Integer>(env, x));
    BOOST_CHECK_EQUAL(cast<Integer>(env, x)->value, 0);
}

BOOST_AUTO_TEST_CASE(typedVariables1)
{
    Environment env;
    Parser p;

    auto parsed = p.parse("y : int = 0", env);
    execute(env, parsed);//
    auto y = env.getEqual("y");
    BOOST_REQUIRE(checkType<Integer>(env, y));
    BOOST_CHECK_EQUAL(cast<Integer>(env, y)->value, 0);
}

BOOST_AUTO_TEST_CASE(typedVariables2)
{
    Environment env;
    Parser p;

    auto parsed = p.parse("int (x0 = 0)", env);
    execute(env, parsed);//
    auto x0 = env.getEqual("x0");
    BOOST_REQUIRE(checkType<Integer>(env, x0));
    BOOST_CHECK_EQUAL(cast<Integer>(env, x0)->value, 0);
}

BOOST_AUTO_TEST_CASE(typedVariables3)
{
    Environment env;
    Parser p;

    auto parsed = p.parse("(int x1) = 0", env);
    execute(env, parsed);//
    auto x1 = env.getEqual("x1");
    BOOST_REQUIRE(checkType<Integer>(env, x1));
    BOOST_CHECK_EQUAL(cast<Integer>(env, x1)->value, 0);
}


// recursion test
// re x y = (((x < 11) \ (0)) | ((x > 10) \ (re (x - 1) (print x))) ) x

// BOOST_AUTO_TEST_CASE(simpleMacroMatch)
// {
//     Environment env;
//     Parser p;

//     auto parsed = p.parse("f ('(1 + 2)) = 4", env);
//     parsexecute(env, ed);//
//     parsed = p.parse("f (1 + 2)", env);
//     auto c = parsed;
//     BOOST_CHECK_EQUAL(cast<Integer>(env, c)->value, 4);
//     parsed = p.parse("f (1 + 3)", env);
//     c = parsed;
//     BOOST_REQUIRE(checkType<Void>(env, c));
//     parsed = p.parse("f 3", env);
//     c = parsed;
//     BOOST_REQUIRE(checkType<Void>(env, c));
// }

// TODO: why did I wrote this test?
// BOOST_AUTO_TEST_CASE(simpleMacroMatchVariable)
// {
//     Environment env;
//     Parser p;

//     auto parsed = p.parse("token ('x) = 0", env);
//     parsexecute(env, ed);//
//     parsed = p.parse("token c", env);
//     auto c = parsed;
//     BOOST_CHECK_EQUAL(cast<Integer>(env, c)->value, 0);

// }

// BOOST_AUTO_TEST_CASE(functionMacroMatch)
// {
//     Environment env;
//     Parser p;
 
//     p.parse("identity x = x", env);
//     p.parse("token ('(f x)) = f", env);
//     auto parsed = p.parse("token (identity x)", env);
//     auto c = parsed;
//     BOOST_CHECK_EQUAL(cast<Identifier>(env, c)->name, "identity");
// }

// BOOST_AUTO_TEST_CASE(plusMacroMatch)
// {
//     Environment env;
//     Parser p;
 
//     p.parse("plustest ('(x + y)) = x * 10 + y", env);
//     auto parsed = p.parse("plustest(2 + 3)", env);
//     auto c = parsed;
//     BOOST_CHECK_EQUAL(cast<Integer>(env, c)->value, 23);
// }

BOOST_AUTO_TEST_CASE(partialPlus)
{
    Environment env;
    Parser p;
    {
        auto parsed = p.parse("(2+) 3", env);
        auto c = parsed;
        BOOST_CHECK_EQUAL(cast<Integer>(env, c)->value, 5);
    }
    {
        auto parsed = p.parse("(+3) 2", env);
        auto c = parsed;
        BOOST_CHECK_EQUAL(cast<Integer>(env, c)->value, 5);
    }
}

BOOST_AUTO_TEST_CASE(operatorAsNonInfix)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("(+) 3 2", env);
    auto c = parsed;
    BOOST_CHECK_EQUAL(cast<Integer>(env, c)->value, 5);
}

BOOST_AUTO_TEST_CASE(firstOrderOperator)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("f op x y = op x y", env);
    execute(env, parsed);
    auto c = p.parse("f (+) 2 3", env);
    BOOST_REQUIRE(checkType<Integer>(env, c));
    BOOST_CHECK_EQUAL(cast<Integer>(env, c)->value, 5);
}

BOOST_AUTO_TEST_CASE(firstOrderAsNonInfix)
{
    return; //unimplemented
    Environment env;
    Parser p;
    {
        auto parsed = p.parse("(=) x 0", env);
        auto c = parsed;
        // BOOST_CHECK_EQUAL(cast<Integer>(env, c)->value, 5);
        auto x = env.getEqual("x");
        BOOST_CHECK_EQUAL(cast<Integer>(env, x)->value, 0);
    }
    {
        auto parsed = p.parse("(=) y 0", env);
        auto c = parsed;
        // BOOST_CHECK_EQUAL(cast<Integer>(env, c)->value, 5);
        auto x = env.getEqual("y");
        BOOST_CHECK_EQUAL(cast<Integer>(env, x)->value, 0);
    }
}

BOOST_AUTO_TEST_CASE(functionOverloading)
{
    return; // unimplemented
    Environment env;
    Parser p;
    auto parsed = p.parse("f 1 = 1", env);
    execute(env, parsed);//
    auto applied0 = p.parse("f 1", env);
    BOOST_CHECK_EQUAL(cast<Integer>(env, applied0)->value, 1);
    auto applied1 = p.parse("f 2", env);
    BOOST_CHECK(cast<Void>(env, applied1));
    parsed = p.parse("f 2 = 2", env);
    execute(env, parsed);//
    auto applied2 = p.parse("f 1", env);
    BOOST_CHECK_EQUAL(cast<Integer>(env, applied2)->value, 1);
    auto applied3 = p.parse("f 2", env);
    BOOST_CHECK_EQUAL(cast<Integer>(env, applied3)->value, 2);
    auto applied4 = p.parse("f 3", env);
    BOOST_CHECK(cast<Void>(env, applied4));
}

BOOST_AUTO_TEST_CASE(unionAsFunctionArgumentSimple)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("f (1 | 2) = 3", env);
    execute(env, parsed);//
    auto applied0 = p.parse("f 1", env);
    BOOST_CHECK_EQUAL(cast<Integer>(env, applied0)->value, 3);
    auto applied1 = p.parse("f 2", env);
    BOOST_CHECK_EQUAL(cast<Integer>(env, applied1)->value, 3);
    auto applied2 = p.parse("f 3", env);
    BOOST_REQUIRE(checkType<Void>(env, applied2));
}

BOOST_AUTO_TEST_CASE(simpleComplementAsArgument)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("f (!1) = 3", env);
    execute(env, parsed);//
    auto applied0 = p.parse("f 2", env);
    BOOST_CHECK_EQUAL(cast<Integer>(env, applied0)->value, 3);
    auto applied2 = p.parse("f 1", env);
    BOOST_REQUIRE(checkType<Void>(env, applied2));
}

BOOST_AUTO_TEST_CASE(simpleClosure0)
{
    Environment env;
    Parser p;
    {
        auto parsed = p.parse("(x => x) 3", env);
        auto c = parsed;
        BOOST_CHECK_EQUAL(cast<Integer>(env, c)->value, 3);
    }
}

BOOST_AUTO_TEST_CASE(simpleClosure1)
{
    Environment env;
    Parser p;
    {
        auto parsed = p.parse("(3 => 3) 3", env);
        auto c = parsed;
        BOOST_CHECK_EQUAL(cast<Integer>(env, c)->value, 3);
    }
}

BOOST_AUTO_TEST_CASE(simpleClosure2)
{
    Environment env;
    Parser p;
    {
        auto parsed = p.parse("(2 => 3) 3", env);
        auto c = parsed;
        BOOST_REQUIRE(checkType<Void>(env, c));
    }
}

BOOST_AUTO_TEST_CASE(noEnvironmentSharing)
{
    Environment env;
    Parser p;
    {
        auto parsed = p.parse("y = 1", env);
        execute(env, parsed);//
        parsed = p.parse("x = ((+1) y) + ((+1) y)", env);
        execute(env, parsed);//
        auto x = env.getEqual("x");
        BOOST_REQUIRE(checkType<Integer>(env, x));
        BOOST_CHECK_EQUAL(cast<Integer>(env, x)->value, 4);
        auto y = env.getEqual("y");
        BOOST_REQUIRE(checkType<Integer>(env, y));
        BOOST_CHECK_EQUAL(cast<Integer>(env, y)->value, 1);
    }
    {
        auto parsed = p.parse("z = 2", env);
        execute(env, parsed);//
        parsed = p.parse("w = (int z) + (int z)", env);
        execute(env, parsed);//
        auto x = env.getEqual("w");
        BOOST_REQUIRE(checkType<Integer>(env, x));
        BOOST_CHECK_EQUAL(cast<Integer>(env, x)->value, 4);
    }
}

BOOST_AUTO_TEST_CASE(complexClosure0)
{
    Environment env;
    Parser p;
    {
        auto parsed = p.parse("(int x => x) 3", env);
        auto c = parsed;
        BOOST_CHECK_EQUAL(cast<Integer>(env, c)->value, 3);
    }
}

BOOST_AUTO_TEST_CASE(complexClosure1)
{
    Environment env;
    Parser p;
    {
        auto parsed = p.parse("(2|3 => 3) 3", env);
        auto c = parsed;
        BOOST_CHECK_EQUAL(cast<Integer>(env, c)->value, 3);
    }
}

BOOST_AUTO_TEST_CASE(complexClosure2)
{
    Environment env;
    Parser p;
    {
        auto parsed = p.parse("(1|2 => 3) 3", env);
        auto c = parsed;
        BOOST_REQUIRE(checkType<Void>(env, c));
    }
}

BOOST_AUTO_TEST_CASE(complexClosure3)
{
    Environment env;
    Parser p;
    {
        auto parsed = p.parse("(int x => 3) \"abc\"", env);
        auto c = parsed;
        BOOST_REQUIRE(checkType<Void>(env, c));
    }
}

BOOST_AUTO_TEST_CASE(closurePredicate0)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("x : (x => (x = 0))", env);
    execute(env, parsed);//
    parsed = p.parse("x = 0", env);
    execute(env, parsed);//
    auto x = env.getEqual("x");
    BOOST_REQUIRE(checkType<Integer>(env, x));
    BOOST_CHECK_EQUAL(cast<Integer>(env, x)->value, 0);
}

BOOST_AUTO_TEST_CASE(closurePredicate1)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("x : (x => (x = 0))", env);
    execute(env, parsed);//
    parsed = p.parse("x = 1", env);
    execute(env, parsed);//
    auto x = env.getEqual("x");
    BOOST_REQUIRE(checkType<Void>(env, x));
}

BOOST_AUTO_TEST_CASE(intersectionMorphism)
{
    Environment env;
    Parser p;
    {
        auto x = p.parse("(>0) 0", env);
        BOOST_REQUIRE(checkType<Void>(env, x));
    }
    {
        auto x = p.parse("(>0) 1", env);
        BOOST_CHECK_EQUAL(cast<Integer>(env, x)->value, 1);
    }
    {
        auto x = p.parse("((>0) & (>0)) 0", env);
        BOOST_REQUIRE(checkType<Void>(env, x));
    }
    {
        auto x = p.parse("((>0) & (>0)) 1", env);
        BOOST_CHECK_EQUAL(cast<Integer>(env, x)->value, 1);
    }
    {
        auto x = p.parse("((>0) & (<10)) 1", env);
        BOOST_CHECK_EQUAL(cast<Integer>(env, x)->value, 1);
    }
    {
        auto x = p.parse("((>0) & (<10)) 11", env);
        BOOST_REQUIRE(checkType<Void>(env, x));
    }
}

BOOST_AUTO_TEST_CASE(closureIntersection)
{
   Environment env;
   Parser p;
   {
       auto parsed = p.parse("((1 => 1) & (1 => 1)) 1", env);
       auto c = parsed;
       BOOST_CHECK_EQUAL(cast<Integer>(env, c)->value, 1);
   }
   {
       auto parsed = p.parse("((int x => 1) & (1 => 1)) 1", env);
       auto c = parsed;
       BOOST_CHECK_EQUAL(cast<Integer>(env, c)->value, 1);
   }
   {
       auto parsed = p.parse("((int x => 1) & (2 => 1)) 1", env);
       auto c = parsed;
       BOOST_REQUIRE(checkType<Void>(env, c));
   }
   {
       auto parsed = p.parse("((int x => 1) & (1 => 1)) 2", env);
       auto c = parsed;
       BOOST_REQUIRE(checkType<Void>(env, c));
   }
}

BOOST_AUTO_TEST_CASE(intersectionType0)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("x : ((>0) & (<10))", env);
    execute(env, parsed);//
    parsed = p.parse("x = 5", env);
    execute(env, parsed);//
    auto x = env.getEqual("x");
    BOOST_REQUIRE(checkType<Integer>(env, x));
    BOOST_CHECK_EQUAL(cast<Integer>(env, x)->value, 5);
}
BOOST_AUTO_TEST_CASE(intersectionType1)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("x : ((>0) & (<10))", env);
    execute(env, parsed);//
    parsed = p.parse("x = 0", env);
    execute(env, parsed);//
    auto x = env.getEqual("x");
    BOOST_REQUIRE(checkType<Void>(env, x));
}

BOOST_AUTO_TEST_CASE(intersectionType2)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("x : ((>0) & (<10))", env);
    execute(env, parsed);//
    parsed = p.parse("x = 10", env);
    execute(env, parsed);//
    auto x = env.getEqual("x");
    BOOST_REQUIRE(checkType<Void>(env, x));
}


BOOST_AUTO_TEST_CASE(simpleInverseFunction)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("f (int x) = x + 1", env);
    execute(env, parsed);//
    auto applied = p.parse("(inverse f) 0", env);
    BOOST_CHECK_EQUAL(cast<Integer>(env, applied)->value, -1);
}

BOOST_AUTO_TEST_CASE(evaluatingInverseFunction0)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("f x = x + 10", env);
    execute(env, parsed);//
    parsed = p.parse("m (int x) = f 10 + f 17", env);
    execute(env, parsed);//
    auto applied = p.parse("(inverse m) 47", env);
    BOOST_REQUIRE(checkType<ValueInSet>(env, applied));
}

BOOST_AUTO_TEST_CASE(evaluatingInverseFunction1)
{
    Environment env;
    env.toggleDebugPrint();
    Parser p;
    auto parsed = p.parse("f x = x + 10", env);
    execute(env, parsed);//
    parsed = p.parse("m (int x) = f 10 + f 17", env);
    execute(env, parsed);//
    auto applied = p.parse("(inverse m) 46", env);
    BOOST_REQUIRE(checkType<Void>(env, applied));
}

BOOST_AUTO_TEST_CASE(evaluatingInverseFunction2)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("f x = x + 10", env);
    execute(env, parsed);//
    parsed = p.parse("n 0 = f 10 + f 17", env);
    execute(env, parsed);//
    auto applied = p.parse("(inverse n) 47", env);
    BOOST_CHECK_EQUAL(cast<Integer>(env, applied)->value, 0);
}


BOOST_AUTO_TEST_CASE(evaluatingInverseFunction3)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("f x = x + 10", env);
    execute(env, parsed);//
    parsed = p.parse("n 0 = f 10 + f 17", env);
    execute(env, parsed);//
    auto applied = p.parse("(inverse n) 46", env);
    BOOST_REQUIRE(checkType<Void>(env, applied));
}

// BOOST_AUTO_TEST_CASE(unionType)
// {
//     {
//         Environment env;
//         Parser p;
//         auto parsed = p.parse("x : ((=0) | (=5))", env);
//         execute(env, parsed);//
//         parsed = p.parse("x = 5", env);
//         execute(env, parsed);//
//         auto x = env.getEqual("x");
//         BOOST_REQUIRE(checkType<Integer>(env, x));
//         BOOST_CHECK_EQUAL(cast<Integer>(env, x)->value, 5);
//     }
//     {
//         Environment env;
//         Parser p;
//         auto parsed = p.parse("x : ((=0) | (=5))", env);
//         execute(env, parsed);//
//         parsed = p.parse("x = 0", env);
//         execute(env, parsed);//
//         auto x = env.getEqual("x");
//         BOOST_REQUIRE(checkType<Integer>(env, x));
//         BOOST_CHECK_EQUAL(cast<Integer>(env, x)->value, 0);
//     }
//     {
//         Environment env;
//         Parser p;
//         auto parsed = p.parse("x : ((=1) | (=2))", env);
//         execute(env, parsed);//
//         parsed = p.parse("x = 0", env);
//         execute(env, parsed);//
//         auto x = env.getEqual("x");
//         BOOST_REQUIRE(checkType<Void>(env, x));
//     }
//     {
//         Environment env;
//         Parser p;
//         auto parsed = p.parse("x : ((=9) | (=11))", env);
//         execute(env, parsed);//
//         parsed = p.parse("x = 10", env);
//         execute(env, parsed);//
//         auto x = env.getEqual("x");
//         BOOST_REQUIRE(checkType<Void>(env, x));
//     }
// }

// BOOST_AUTO_TEST_CASE(closureIntersectionType)
// {
//     {
//         Environment env;
//         Parser p;
//         auto parsed = p.parse("x : ((x => (x > 0)) & (x => (x < 10)))", env);
//         execute(env, parsed);//
//         parsed = p.parse("x = 5", env);
//         execute(env, parsed);//
//         auto x = env.getEqual("x");
//         BOOST_REQUIRE(checkType<Integer>(env, x));
//         BOOST_CHECK_EQUAL(cast<Integer>(env, x)->value, 5);
//     }
//     {
//         Environment env;
//         Parser p;
//         auto parsed = p.parse("x : ((x => (x > 0)) & (x => (x < 10)))", env);
//         execute(env, parsed);//
//         parsed = p.parse("x = 0", env);
//         execute(env, parsed);//
//         auto x = env.getEqual("x");
//         BOOST_REQUIRE(checkType<Void>(env, x));
//     }
//     {
//         Environment env;
//         Parser p;
//         auto parsed = p.parse("x : ((x => int x) & (>0))", env);
//         execute(env, parsed);//
//         parsed = p.parse("x = 0", env);
//         execute(env, parsed);//
//         auto x = env.getEqual("x");
//         BOOST_REQUIRE(checkType<Void>(env, x));
//     }
// }

BOOST_AUTO_TEST_CASE(simpleLet)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("f x = let (a = x + 2) a", env);
    execute(env, parsed);//
    parsed = p.parse("z = f 1", env);
    execute(env, parsed);//
    auto x = env.getEqual("z");
    BOOST_REQUIRE(checkType<Integer>(env, x));
    BOOST_CHECK_EQUAL(cast<Integer>(env, x)->value, 3);
}

BOOST_AUTO_TEST_CASE(letSharing)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("let (x = 3)", env);
    execute(env, parsed);//
    auto x = env.getEqual("x");
    BOOST_REQUIRE(checkType<Integer>(env, x));
    BOOST_CHECK_EQUAL(cast<Integer>(env, x)->value, 3);
}

BOOST_AUTO_TEST_CASE(contains0)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("x : (=(1 | 2))", env);
    execute(env, parsed);//
    parsed = p.parse("x = 1", env);
    execute(env, parsed);//
    auto x = env.getEqual("x");
    BOOST_REQUIRE(checkType<Integer>(env, x));
    BOOST_CHECK_EQUAL(cast<Integer>(env, x)->value, 1);
}

BOOST_AUTO_TEST_CASE(contains1)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("x : (=(1 | 2 | (2 + 1) | (2 + 2) | 5))", env);
    execute(env, parsed);//
    parsed = p.parse("x = 3", env);
    execute(env, parsed);//
    auto x = env.getEqual("x");
    BOOST_REQUIRE(checkType<Integer>(env, x));
    BOOST_CHECK_EQUAL(cast<Integer>(env, x)->value, 3);
}
