#define BOOST_TEST_MODULE ParserTests
#include <boost/test/unit_test.hpp>
#include <iostream>
#include "System/Environment.h"
#include "System/Cast.h"
#include "Expressions/Identifier.h"
#include "Expressions/Operation.h"
#include "Expressions/Application.h"
#include "Expressions/Any.h"
#include "Expressions/Void.h"
#include "Parser/Parser.h"
#include "Expressions/Integer.h"
#include "Expressions/Then.h"
#include "Expressions/Addition.h"
#include "Expressions/DefaultOperator.h"

BOOST_AUTO_TEST_CASE(VariableTest)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("x", env);

    auto x = castNoEval<Identifier>(parsed);
    BOOST_REQUIRE(x != nullptr);
    BOOST_CHECK_EQUAL(x->name, "x");
}

BOOST_AUTO_TEST_CASE(ApplicationTest)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("f x", env);

    BOOST_CHECK(checkTypeNoEval<Operation>(parsed));
    auto op = castNoEval<Operation>(parsed);
    BOOST_CHECK(checkTypeNoEval<Application>(op->op));
    BOOST_CHECK(checkTypeNoEval<Identifier>(op->left));
    BOOST_CHECK_EQUAL(castNoEval<Identifier>(op->left)->name, "f");
    BOOST_CHECK(checkTypeNoEval<Identifier>(op->right));
    BOOST_CHECK_EQUAL(castNoEval<Identifier>(op->right)->name, "x");
}

BOOST_AUTO_TEST_CASE(SimpleBracesTest)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("(f x)", env);

    BOOST_CHECK(checkTypeNoEval<Operation>(parsed));
    auto op = castNoEval<Operation>(parsed);
    BOOST_CHECK(checkTypeNoEval<Application>(op->op));
    BOOST_CHECK(checkTypeNoEval<Identifier>(op->left));
    BOOST_CHECK_EQUAL(castNoEval<Identifier>(op->left)->name, "f");
    BOOST_CHECK(checkTypeNoEval<Identifier>(op->right));
    BOOST_CHECK_EQUAL(castNoEval<Identifier>(op->right)->name, "x");
}

BOOST_AUTO_TEST_CASE(VoidTest)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("()", env);

    BOOST_CHECK(checkTypeNoEval<Void>(parsed));
}

BOOST_AUTO_TEST_CASE(BracesTest)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("g (f x)", env);

    BOOST_CHECK(checkTypeNoEval<Operation>(parsed));
    auto op = castNoEval<Operation>(parsed);
    BOOST_CHECK(checkTypeNoEval<Application>(op->op));
    BOOST_CHECK_EQUAL(castNoEval<Identifier>(op->left)->name, "g");


    auto f_x = castNoEval<Operation>(op->right);
    BOOST_CHECK(checkTypeNoEval<Application>(f_x->op));
    BOOST_CHECK(checkTypeNoEval<Identifier>(f_x->left));
    BOOST_CHECK_EQUAL(castNoEval<Identifier>(f_x->left)->name, "f");
    BOOST_CHECK(checkTypeNoEval<Identifier>(f_x->right));
    BOOST_CHECK_EQUAL(castNoEval<Identifier>(f_x->right)->name, "x");
}

BOOST_AUTO_TEST_CASE(MultipleApplication)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("f x y (a b) z", env);

    BOOST_CHECK(checkTypeNoEval<Operation>(parsed));

    auto f_x_y_oa_bc_z = castNoEval<Operation>(parsed);
    BOOST_CHECK(checkTypeNoEval<Application>(f_x_y_oa_bc_z->op));
    BOOST_CHECK(checkTypeNoEval<Operation>(f_x_y_oa_bc_z->left));
    BOOST_CHECK(checkTypeNoEval<Identifier>(f_x_y_oa_bc_z->right));
    BOOST_CHECK_EQUAL(castNoEval<Identifier>(f_x_y_oa_bc_z->right)->name, "z");

    auto f_x_y_oa_bc = castNoEval<Operation>(f_x_y_oa_bc_z->left);
    BOOST_CHECK(checkTypeNoEval<Application>(f_x_y_oa_bc->op));
    BOOST_CHECK(checkTypeNoEval<Operation>(f_x_y_oa_bc->left));
    BOOST_CHECK(checkTypeNoEval<Operation>(f_x_y_oa_bc->right));

    auto f_x_y = castNoEval<Operation>(f_x_y_oa_bc->left);
    BOOST_CHECK(checkTypeNoEval<Application>(f_x_y->op));
    BOOST_CHECK(checkTypeNoEval<Operation>(f_x_y->left));
    BOOST_CHECK(checkTypeNoEval<Identifier>(f_x_y->right));
    BOOST_CHECK_EQUAL(castNoEval<Identifier>(f_x_y->right)->name, "y");

    auto a_b = castNoEval<Operation>(f_x_y_oa_bc->right);
    BOOST_CHECK(checkTypeNoEval<Application>(a_b->op));
    BOOST_CHECK(checkTypeNoEval<Identifier>(a_b->left));
    BOOST_CHECK_EQUAL(castNoEval<Identifier>(a_b->left)->name, "a");
    BOOST_CHECK(checkTypeNoEval<Identifier>(a_b->right));
    BOOST_CHECK_EQUAL(castNoEval<Identifier>(a_b->right)->name, "b");

    auto f_x = castNoEval<Operation>(f_x_y->left);
    BOOST_CHECK(checkTypeNoEval<Application>(f_x->op));
    BOOST_CHECK(checkTypeNoEval<Identifier>(f_x->left));
    BOOST_CHECK_EQUAL(castNoEval<Identifier>(f_x->left)->name, "f");
    BOOST_CHECK(checkTypeNoEval<Identifier>(f_x->right));
    BOOST_CHECK_EQUAL(castNoEval<Identifier>(f_x->right)->name, "x");
}

BOOST_AUTO_TEST_CASE(ParenthesesParse)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("(f x y (( (a) ((b)))) (((z))))", env);

    BOOST_CHECK(checkTypeNoEval<Operation>(parsed));

    auto f_x_y_oa_bc_z = castNoEval<Operation>(parsed);
    BOOST_CHECK(checkTypeNoEval<Application>(f_x_y_oa_bc_z->op));
    BOOST_CHECK(checkTypeNoEval<Operation>(f_x_y_oa_bc_z->left));
    BOOST_CHECK(checkTypeNoEval<Identifier>(f_x_y_oa_bc_z->right));
    BOOST_CHECK_EQUAL(castNoEval<Identifier>(f_x_y_oa_bc_z->right)->name, "z");

    auto f_x_y_oa_bc = castNoEval<Operation>(f_x_y_oa_bc_z->left);
    BOOST_CHECK(checkTypeNoEval<Application>(f_x_y_oa_bc->op));
    BOOST_CHECK(checkTypeNoEval<Operation>(f_x_y_oa_bc->left));
    BOOST_CHECK(checkTypeNoEval<Operation>(f_x_y_oa_bc->right));

    auto f_x_y = castNoEval<Operation>(f_x_y_oa_bc->left);
    BOOST_CHECK(checkTypeNoEval<Application>(f_x_y->op));
    BOOST_CHECK(checkTypeNoEval<Operation>(f_x_y->left));
    BOOST_CHECK(checkTypeNoEval<Identifier>(f_x_y->right));
    BOOST_CHECK_EQUAL(castNoEval<Identifier>(f_x_y->right)->name, "y");

    auto a_b = castNoEval<Operation>(f_x_y_oa_bc->right);
    BOOST_CHECK(checkTypeNoEval<Application>(a_b->op));
    BOOST_CHECK(checkTypeNoEval<Identifier>(a_b->left));
    BOOST_CHECK_EQUAL(castNoEval<Identifier>(a_b->left)->name, "a");
    BOOST_CHECK(checkTypeNoEval<Identifier>(a_b->right));
    BOOST_CHECK_EQUAL(castNoEval<Identifier>(a_b->right)->name, "b");

    auto f_x = castNoEval<Operation>(f_x_y->left);
    BOOST_CHECK(checkTypeNoEval<Application>(f_x->op));
    BOOST_CHECK(checkTypeNoEval<Identifier>(f_x->left));
    BOOST_CHECK_EQUAL(castNoEval<Identifier>(f_x->left)->name, "f");
    BOOST_CHECK(checkTypeNoEval<Identifier>(f_x->right));
    BOOST_CHECK_EQUAL(castNoEval<Identifier>(f_x->right)->name, "x");
}

BOOST_AUTO_TEST_CASE(XPlusY)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("x + y", env);

    BOOST_CHECK(checkTypeNoEval<Operation>(parsed));
    auto op = castNoEval<Operation>(parsed);
    BOOST_CHECK(checkTypeNoEval<Addition>(op->op));
    BOOST_CHECK(checkTypeNoEval<Identifier>(op->left));
    BOOST_CHECK_EQUAL(castNoEval<Identifier>(op->left)->name, "x");
    BOOST_CHECK(checkTypeNoEval<Identifier>(op->right));
    BOOST_CHECK_EQUAL(castNoEval<Identifier>(op->right)->name, "y");
}

BOOST_AUTO_TEST_CASE(NumbersOperation)
{
    Environment env;
    Parser p;
    auto parsed = p.parse("1 + 2", env);

    BOOST_CHECK(checkTypeNoEval<Operation>(parsed));
    auto op = castNoEval<Operation>(parsed);
    BOOST_CHECK(checkTypeNoEval<Addition>(op->op));
    BOOST_CHECK(checkTypeNoEval<Integer>(op->left));
    BOOST_CHECK_EQUAL(castNoEval<Integer>(op->left)->value, 1);
    BOOST_CHECK(checkTypeNoEval<Integer>(op->right));
    BOOST_CHECK_EQUAL(castNoEval<Integer>(op->right)->value, 2);
}

BOOST_AUTO_TEST_CASE(HighPriorityUsage)
{
    return; // unimplemented
    Environment env;
    Parser p;
    auto parsed = p.parse("f 2+3 sin(x)", env);
}

BOOST_AUTO_TEST_CASE(MultilineApplication)
{
    return; // unimplemented
    Environment env;
    Parser p;
    auto parsed = p.parse("f\n"
                          "  x", env);

    BOOST_CHECK(checkTypeNoEval<Operation>(parsed));
    auto op = castNoEval<Operation>(parsed);
    BOOST_CHECK(checkTypeNoEval<Application>(op->op));
    BOOST_CHECK(checkTypeNoEval<Identifier>(op->left));
    BOOST_CHECK_EQUAL(castNoEval<Identifier>(op->left)->name, "f");
    BOOST_CHECK(checkTypeNoEval<Identifier>(op->right));
    BOOST_CHECK_EQUAL(castNoEval<Identifier>(op->right)->name, "x");
}

BOOST_AUTO_TEST_CASE(MultilineApplication2)
{
    return; // unimplemented
    Environment env;
    Parser p;
    auto parsed = p.parse("f x\n"
                          "  y", env);

    BOOST_CHECK(checkTypeNoEval<Operation>(parsed));
    auto op = castNoEval<Operation>(parsed);
    BOOST_CHECK(checkTypeNoEval<Application>(op->op));
    BOOST_CHECK(checkTypeNoEval<Identifier>(op->left));
    BOOST_CHECK_EQUAL(castNoEval<Identifier>(op->left)->name, "f");
    BOOST_CHECK(checkTypeNoEval<Identifier>(op->right));
    BOOST_CHECK_EQUAL(castNoEval<Identifier>(op->right)->name, "x");
}

BOOST_AUTO_TEST_CASE(MultilineApplicationSeparate)
{
    return; // unimplemented
    Environment env;
    Parser p;
    auto parsed = p.parse("f\n"
                          "  x"
                          "y", env);

    BOOST_CHECK(checkTypeNoEval<Operation>(parsed));
    auto op = castNoEval<Operation>(parsed);
    BOOST_CHECK(checkTypeNoEval<Application>(op->op));
    BOOST_CHECK(checkTypeNoEval<Identifier>(op->left));
    BOOST_CHECK_EQUAL(castNoEval<Identifier>(op->left)->name, "f");
    BOOST_CHECK(checkTypeNoEval<Identifier>(op->right));
    BOOST_CHECK_EQUAL(castNoEval<Identifier>(op->right)->name, "x");
}

BOOST_AUTO_TEST_CASE(AdditionSection)
{
    return; // unimplemented
    Environment env;
    Parser p;
    auto parsed = p.parse("1 + 2\n"
                          "    3", env);

    BOOST_CHECK(checkTypeNoEval<Operation>(parsed));
    auto op = castNoEval<Operation>(parsed);
    BOOST_CHECK(checkTypeNoEval<Addition>(op->op));
    BOOST_CHECK(checkTypeNoEval<Integer>(op->left));
    BOOST_CHECK_EQUAL(castNoEval<Integer>(op->left)->value, 1);
    BOOST_CHECK(checkTypeNoEval<Integer>(op->right));
    BOOST_CHECK(checkTypeNoEval<Operation>(op->right));
    op = castNoEval<Operation>(op->right);
    BOOST_CHECK(checkTypeNoEval<DefaultOperator>(op->op));
    BOOST_CHECK(checkTypeNoEval<Integer>(op->left));
    BOOST_CHECK_EQUAL(castNoEval<Integer>(op->left)->value, 2);
    BOOST_CHECK(checkTypeNoEval<Integer>(op->right));
    BOOST_CHECK_EQUAL(castNoEval<Integer>(op->right)->value, 3);
}
