#define BOOST_TEST_MODULE ParserTests
#include <boost/test/unit_test.hpp>

int testOfTests2(int i, int j)
{
    return i + j;
}

BOOST_AUTO_TEST_CASE(universeInOrder2)
{
    BOOST_CHECK(testOfTests2(2, 2) == 4);
}
