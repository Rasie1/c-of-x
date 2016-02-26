#include "Less.h"
#include "Equality.h"
#include "Identifier.h"
#include "Environment.h"
#include "Operation.h"
#include "Intersection.h"
#include "Void.h"
#include "Lambda.h"
#include "Integer.h"


bool LessThan::holds(ExpPtrArg e, const Environment& env) const
{
    return *value == *e;
}

bool LessThan::operator==(const Expression& other) const
{
    if (typeid(*this) == typeid(other))
    {
        auto x = static_cast<const LessThan&>(other);
        return value == x.value;
    }
    else
        return false;
}

std::string LessThan::show() const
{
    return defaultName + "(" + value->show() + ")";
}

const std::string LessThan::defaultName = "lessThan";

ExpPtr LessThan::intersect(ExpPtrArg other, const Environment& env)
{
    if (typeid(*this) == typeid(*other))
    {
        auto p = s_cast<LessThan>(other);
        if (typeid(*value) == typeid(Integer) &&
            typeid(*(p->value)) == typeid(Integer))
        {
            auto v1 = s_cast<Integer>(value);
            auto v2 = s_cast<Integer>(p->value);
            return make_ptr<LessThan>(make_ptr<Integer>(std::min(v1->value, v2->value)));
        }
        return make_ptr<Void>();
//        return make_ptr<LessThan>(make_ptr<Integer>(value));
    }
    return make_ptr<Operation>(make_ptr<Intersection>(), value, other);
}

Less::Less()
    : Operator(true, 2)
{

}

static bool operateHelper(ExpPtrArg first,
                          ExpPtrArg second,
                          Environment& env)
{
    auto lvalue = first;
    auto rvalue = second;
    if (typeid(*lvalue) != typeid(Identifier))
        while (lvalue->unwind(lvalue, rvalue, env));

    if (typeid(*lvalue) == typeid(Identifier))
    {
        auto id = s_cast<Identifier>(lvalue);

        // For recursion
        if (typeid(*rvalue) == typeid(Operation))
        {
            auto operation = s_cast<Operation>(rvalue);
            if (operation && (typeid(*(operation->op)) == typeid(Lambda)))
            {
                auto newEnv = env;
                newEnv.addEqual(id, rvalue);
                env.addEqual(id, rvalue->eval(newEnv));
                return true;
            }
        }

        rvalue = Identifier::unwrapIfId(rvalue, env);

        env.addEqual(id, rvalue->eval(env));
        return true;
    }
    else
    {
        auto le = first->eval(env);
        auto re = second->eval(env);
        re = Identifier::unwrapIfId(re, env);

        return *le == *re;
    }
}

ExpPtr Less::operate(ExpPtrArg first,
                     ExpPtrArg second,
                     Environment& env) const

{
    auto envl = env;
    auto envr = env;
#ifdef DEBUG_EVAL
    DEBUG_INDENTATION;
    std::cout << "Less: (" << first->show() << ") < (" << second->show() << ")"
              << std::endl;
    DEBUG_INDENTATION;
    std::cout << "L R:" << std::endl;
    DEBUG_INDENT_INCR;
#endif
    auto l = operateHelper(first, second, envl);
#ifdef DEBUG_EVAL
    DEBUG_INDENT_DECR;
    DEBUG_INDENTATION;
    std::cout << "R L:" << std::endl;
    DEBUG_INDENT_INCR;
#endif
    auto r = operateHelper(second, first, envr);
#ifdef DEBUG_EVAL
    DEBUG_INDENT_DECR;
#endif

    ExpPtr ret;
    if (l)
    {
        env = envl;
        ret = first;
    }
    else if (r)
    {
        env = envr;
        ret = second;
    }
    else
        return make_ptr<Void>();


    return ret;
}

std::string Less::show() const
{
    return Equality::defaultName;
}

const std::string Less::defaultName = "<";
