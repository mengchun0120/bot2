#ifndef INCLUDE_BOT_VALIDATOR
#define INCLUDE_BOT_VALIDATOR

#include <functional>
#include <string>
#include <sstream>

namespace bot {

class Validator {
public:
    using VerifyFunc = std::function<bool(void)>;
    using RuleFunc = std::function<std::string(const std::string&)>;

    Validator()
    {}

    Validator(const Validator &v)
        : m_verifyFunc(v.m_verifyFunc)
        , m_ruleFunc(v.m_ruleFunc)
    {}

    ~Validator()
    {}

    void setVerifyFunc(VerifyFunc func)
    {
        m_verifyFunc = func;
    }

    bool verify() const
    {
        if (m_verifyFunc)
        {
            return m_verifyFunc();
        }

        return true;
    }

    void setRuleFunc(RuleFunc func)
    {
        m_ruleFunc = func;
    }

    std::string getRule(const std::string &name) const
    {
        std::string rule;

        if (m_ruleFunc)
        {
            rule = m_ruleFunc(name);
        }

        return rule;
    }

private:
    VerifyFunc m_verifyFunc;
    RuleFunc m_ruleFunc;
};

template <typename T>
Validator eq(const T &t, const T &e)
{
    Validator v;

    v.setVerifyFunc(
        [&]()->bool
        {
            return t == e;
        }
    );

    v.setRuleFunc(
        [&](const std::string &name)->std::string
        {
            std::ostringstream oss;
            oss << "(" << name << " == " << e << ")";
            return oss.str();
        }
    );

    return v;
}

template <typename T>
Validator ne(const T &t, const T &e)
{
    Validator v;

    v.setVerifyFunc(
        [&]()->bool
        {
            return t != e;
        }
    );

    v.setRuleFunc(
        [&](const std::string &name)->std::string
        {
            std::ostringstream oss;
            oss << "(" << name << " != " << e << ")";
            return oss.str();
        }
    );

    return v;
}

template <typename T>
Validator lt(const T &t, const T &upper)
{
    Validator v;

    v.setVerifyFunc(
        [&]()->bool
        {
            return t < upper;
        }
    );

    v.setRuleFunc(
        [&](const std::string &name)->std::string
        {
            std::ostringstream oss;
            oss << "(" << name << " < " << upper << ")";
            return oss.str();
        }
    );

    return v;
}

template <typename T>
Validator le(const T &t, const T &upper)
{
    Validator v;

    v.setVerifyFunc(
        [&]()->bool
        {
            return t <= upper;
        }
    );

    v.setRuleFunc(
        [&](const std::string &name)->std::string
        {
            std::ostringstream oss;
            oss << "(" << name << " <= " << upper << ")";
            return oss.str();
        }
    );

    return v;
}

template <typename T>
Validator gt(const T &t, const T &lower)
{
    Validator v;

    v.setVerifyFunc(
        [&]()->bool
        {
            return t > lower;
        }
    );

    v.setRuleFunc(
        [&](const std::string &name)->std::string
        {
            std::ostringstream oss;
            oss << "(" << name << " > " << lower << ")";
            return oss.str();
        }
    );

    return v;
}

template <typename T>
Validator ge(const T &t, const T &lower)
{
    Validator v;

    v.setVerifyFunc(
        [&]()->bool
        {
            return t >= lower;
        }
    );

    v.setRuleFunc(
        [&](const std::string &name)->std::string
        {
            std::ostringstream oss;
            oss << "(" << name << " >= " << lower << ")";
            return oss.str();
        }
    );

    return v;
}

Validator operator&&(const Validator &a, const Validator &b);

Validator operator||(const Validator &a, const Validator &b);

Validator operator!(const Validator &a);

} // end of namespace bot

#endif

