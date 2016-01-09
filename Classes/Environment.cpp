#include "Environment.h"
#include "Expression.h"
#include "TypeError.h"
#include "EnvironmentalVariable.h"

#include "Addition.h"
#include "Subtraction.h"
#include "Multiplication.h"
#include "Assignment.h"
#include "Pair.h"
#include "Void.h"
#include "Function.h"
#include "Lambda.h"
#include "Include.h"
#include "Mutation.h"
#include "Print.h"
#include "Application.h"
#include "Variable.h"
#include "String.h"
#include "Then.h"

#include "EvalDelay.h"
#include "EvalForce.h"

#include <vector>


/*class MatchChain : public Expression,
                   private std::vector<ExpPtr>
{
    typedef std::vector<ExpPtr> Container;
public:
    ExpPtr apply(ExpPtrArg e, Environment*& env) const override
    {
        return find(e);
    }

    void add(ExpPtrArg exp)
    {
        push_back(exp);
    }
    ExpPtr find(ExpPtrArg pat)
    {
        for (auto i = rbegin(); i != rend(); ++i)
        {
            if (!d_cast<String>(i->match(pat)))
                return *i;
        }
        return make_ptr<String>("!No Match!");
    }

    std::string show() const override { return defaultName; }
    static const std::string defaultName;
private:
    using Container::push_back;
    using Container::rbegin;
    using Container::rend;
};


const std::string MatchChain::defaultName = "match_chain";*/






Environment::Environment(EnvironmentalVariable* data, Environment* next)
    : data(data),
      next(next)
{
}

Environment::~Environment()
{
    delete data;
}

void Environment::clear()
{
    delete data;
    data = nullptr;
    if (next == nullptr)
        return;

    next->clear();
    delete next;
    next = nullptr;
}

Environment* Environment::pop()
{
    auto next = this->next;
    delete this;

    return next;
}

ExpPtr Environment::get(ExpPtrArg p)
{
    if (data->match(p, this))
        return data->get();

    if (next == nullptr)
        return nullptr;

    return next->get(p);
}

Environment* Environment::add(ExpPtrArg p,
                              ExpPtrArg e)
{
    auto variable = new EnvironmentalVariable(p, e);
    return new Environment(variable, this);
}

Environment* Environment::loadDefaultVariables()
{
    return add(make_ptr<Variable>(Lambda         ::defaultName), make_ptr<Lambda>())
         ->add(make_ptr<Variable>(Assignment     ::defaultName), make_ptr<Assignment>())
         ->add(make_ptr<Variable>(Then           ::defaultName), make_ptr<Then>())
         ->add(make_ptr<Variable>(EvalForce      ::defaultName), make_ptr<EvalForce>())
         ->add(make_ptr<Variable>(EvalDelay      ::defaultName), make_ptr<EvalDelay>())
         ->add(make_ptr<Variable>(Print          ::defaultName), make_ptr<Print>())
         ->add(make_ptr<Variable>(Include        ::defaultName), make_ptr<Include>())
         ->add(make_ptr<Variable>(Addition       ::defaultName), make_ptr<Addition>())
         //->add(make_ptr<Variable>(Mutation       ::defaultName), make_ptr<Mutation>())
         ->add(make_ptr<Variable>(Subtraction    ::defaultName), make_ptr<Subtraction>())
         //->add(make_ptr<Variable>(Multiplication ::defaultName), make_ptr<Multiplication>())
         ->add(make_ptr<Variable>(Assignment     ::defaultName), make_ptr<Assignment>())
         ->add(make_ptr<Variable>(Pair           ::defaultName), make_ptr<Pair>())
         ;
}

Environment* Environment::create()
{
    auto firstVariable = new EnvironmentalVariable(
                make_ptr<Variable>(Void::defaultName),
                make_ptr<Void>());
    auto environment = new Environment(firstVariable, nullptr);
    return environment->loadDefaultVariables();
}

bool Environment::compareOperators(ExpPtrArg first,
                                   ExpPtrArg second)
{
    int firstValue = 0, secondValue = 1;
    bool isFirstRightAssociative = false;

    std::shared_ptr<Operator> firstOperator, secondOperator;
    //std::shared_ptr<Variable> firstOperator, secondOperator;
    auto env = this;
    firstOperator  = d_cast<Operator>(first->eval(env));
    secondOperator = d_cast<Operator>(second->eval(env));

    if (firstOperator)
    {
        firstValue = firstOperator->priority;
        isFirstRightAssociative = firstOperator->isRightAssociative;
    }
    if (secondOperator)
        secondValue = secondOperator->priority;

    return firstValue < secondValue ||
           !isFirstRightAssociative && (firstValue <= secondValue);
}

std::pair<ExpPtr, ExpPtr> Environment::top()
{
    return std::pair<ExpPtr, ExpPtr>(
                data->get(),
                data->get());
}

Environment* Environment::getNext()
{
    return this->next;
}

ExpPtr Environment::lookup(const std::string& name)
{
    return nullptr;
//    auto exp   = variables.at(name);
//    auto chain = s_cast<MatchChain>(chain);
//    auto match = chain.match(exp);

//    return match;
}

/*
bool PrecedenceTable::more(Operator* first, Operator* second)
{
    return false;
}

void PrecedenceTable::add(PrecedenceInfo* op)
{

}

void PrecedenceTable::remove(PrecedenceInfo* op)
{

}*/

