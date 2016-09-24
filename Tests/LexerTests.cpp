#define BOOST_TEST_MODULE LanguageTests
#include <boost/test/unit_test.hpp>
#include "Lexer.h"

BOOST_AUTO_TEST_CASE(lexer)
{
    Lexer a;
    a.getTokens();
}
