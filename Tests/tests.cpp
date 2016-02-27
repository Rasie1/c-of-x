#define BOOST_TEST_MODULE ParserTests
#include <boost/test/unit_test.hpp>

int testOfTests(int i, int j)
{
    return i + j;
}

BOOST_AUTO_TEST_CASE(universeInOrder)
{
    BOOST_CHECK(testOfTests(2, 2) == 4);
}
