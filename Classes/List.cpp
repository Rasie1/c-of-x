#include "List.h"
#include <string>

namespace Language
{

List::List(Pair* pair)
    : pair(pair)
{

}

Expression* List::head()
{
    return pair->l;
}

Expression* List::tail()
{
    return pair->r;
}

std::string List::toString() const
{
    return "list " + pair->toString();
}


MakeList::MakeList()
{

}

Expression* MakeList::applyConstEnv(Expression* e, Environment* env)
{
    auto pair = dynamic_cast<Pair*>(e);
    if (pair != nullptr)
    {
        return new List(pair);
    }

    return Expression::applyConstEnv(e, env);
}

std::string MakeList::toString() const
{
    return "list";
}



}
