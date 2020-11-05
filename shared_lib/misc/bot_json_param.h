#ifndef INCLUDE_BOT_JSON_PARAM
#define INCLUDE_BOT_JSON_PARAM

#include <string>
#include <rapidjson/document.h>
#include <misc/bot_log.h>

namespace bot {

class JsonParam {
public:
    JsonParam(const std::string& name, bool required=true)
        : m_name(name)
        , m_required(required)
    {}

    JsonParam(const char* name, bool required=true)
        : m_name(name)
    {}

    virtual ~JsonParam()
    {}

    const std::string& getName() const
    {
        return m_name;
    }

    virtual bool parse(const rapidjson::Value& elem) = 0;

protected:
    bool m_required;
    std::string m_name;
};

template <typename T, typename PARSER, typename VALIDATOR>
class TypedJsonParam: public JsonParam {
public:
    TypededJsonParam(T& t, const std::string& name, PARSER parser,
                     VALIDATOR* validator, bool required=true)
        : JsonParam(name, required)
        , m_var(t)
        , m_parser(parser)
        , m_validator(validator)
    {}

    virtual ~TypedJsonParam()
    {}

    T& var()
    {
        return m_var;
    }

    virtual bool parse(const rapidjson::Value& elem) = 0;

protected:
    T& m_var;
    PARSER m_parser;
    VALIDATOR* m_validator;
};

template <typename T, typename PARSER, typename VALIDATOR>
bool TypedJsonParam<T,PARSER,VALIDATOR>::parse(const rapidjson::Value& elem)
{
    if (!m_required)
    {
        return true;
    }

    if (!elem.HasMember(m_name.c_str()))
    {
        LOG_ERROR("%s doesn't exist", m_name.c_str());
        return false;
    }

    if (!m_parser.parse(m_var, m_name.c_str(), elem))
    {
        return false;
    }

    if (
}



} // end of namespace bot

#endif

