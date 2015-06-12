#include "List.h"
#include <string>

namespace Language
{

List::List(const std::list<Expression*>& elements)
    : elements(elements)
{

}

Expression* List::eval(Environment* env)
{
    std::list<Expression*> evaluated;
    for (auto &x : elements)
        evaluated.push_back(x->eval(env));
    auto ret = new List(evaluated);
    if (ret->elements.size() == 1)
        return ret->elements.front();
    return ret;
}

void List::pushBack(Expression* e)
{
    elements.push_back(e);
}

void List::pushFront(Expression* e)
{
    elements.push_front(e);
}

Expression* List::head()
{
    return elements.front();
}

Expression* List::tail()
{
    auto ret = new List(this->elements); //super-slow
    ret->elements.pop_front();
    return ret;
}

std::string List::toString() const
{
    std::string s = "(" + elements.front()->toString();
    for (auto i = next(begin(elements)); i != end(elements); ++i)
        s += ", " + (*i)->toString();
    s += ")";
    return s;
}

}
