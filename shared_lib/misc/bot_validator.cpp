#include <sstream>
#include "misc/bot_validator.h"

namespace bot {

Validator operator&&(const Validator& a, const Validator& b)
{
    Validator v;

    v.setVerifyFunc(
        [&]()->bool
        {
            return a.verify() && b.verify();
        }
    );

    v.setRuleFunc(
        [&](const std::string& name)->std::string
        {
            std::ostringstream oss;
            oss << "(" << a.getRule(name) << " && " << b.getRule(name) << ")";
            return oss.str();
        }
    );

    return v;
}

Validator operator||(const Validator& a, const Validator& b)
{
    Validator v;

    v.setVerifyFunc(
        [&]()->bool
        {
            return a.verify() || b.verify();
        }
    );

    v.setRuleFunc(
        [&](const std::string& name)->std::string
        {
            std::ostringstream oss;
            oss << "(" << a.getRule(name) << " || " << b.getRule(name) << ")";
            return oss.str();
        }
    );

    return v;
}

Validator operator!(const Validator& a)
{
    Validator v;

    v.setVerifyFunc(
        [&]()->bool
        {
            return !a.verify();
        }
    );

    v.setRuleFunc(
        [&](const std::string& name)->std::string
        {
            std::ostringstream oss;
            oss << "!" << a.getRule(name);
            return oss.str();
        }
    );

    return v;
}

} // end of namespace bot
