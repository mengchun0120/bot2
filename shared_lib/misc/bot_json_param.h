#ifndef INCLUDE_BOT_JSON_PARAM
#define INCLUDE_BOT_JSON_PARAM

#include <memory>
#include "misc/bot_json_parser.h"

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

typedef std::shared_ptr<JsonParam> JsonParamPtr;

template <typename T, typename VALIDATOR>
class TypedJsonParam: public JsonParam {
public:
    TypededJsonParam(T& t, const std::string& name,
                     std::shared_ptr<VALIDATOR> validator=nullptr,
                     bool required=true)
        : JsonParam(name, required)
        , m_var(t)
        , m_validator(validator)
    {}

    TypededJsonParam(T& t, const char* name,
                     std::shared_ptr<VALIDATOR> validator=nullptr,
                     bool required=true)
        : JsonParam(name, required)
        , m_var(t)
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
    shared_ptr<VALIDATOR> m_validator;
};

template <typename T, typename VALIDATOR>
bool TypedJsonParam<T,VALIDATOR>::parse(const rapidjson::Value& elem)
{
    if (!JsonParser::parse(m_var, elem, m_name.c_str(), m_required))
    {
        return false;
    }

    if (m_validator && m_validator->validate(m_var))
    {
        return false;
    }

    return false;
}

template <typename T, typename VALIDATOR>
JsonParamPtr getJsonParam(T& t, const std::string& name,
                          VALIDATOR* validator=nullptr,
                          bool required=true)
{
    TypedJsonParam<T,VALIDATOR>* param =
                 new TypedJsonParam<T,VALIDATOR>(t, name, validator, required);

    return JsonParamPtr(param);
}

template <typename T, typename VALIDATOR>
JsonParamPtr getJsonParam(T& t, const char* name,
                          VALIDATOR* validator=nullptr,
                          bool required=true)
{
    TypedJsonParam<T,VALIDATOR>* param =
                 new TypedJsonParam<T,VALIDATOR>(t, name, validator, required);

    return JsonParamPtr(param);
}

} // end of namespace bot

#endif

