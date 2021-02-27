#ifndef INCLUDE_BOT_JSON_PARAM
#define INCLUDE_BOT_JSON_PARAM

#include <memory>
#include "misc/bot_log.h"
#include "misc/bot_validator.h"
#include "misc/bot_json_parser.h"

namespace bot {

class JsonParam {
public:
    JsonParam(const std::string &name, bool required=true)
        : m_name(name)
        , m_required(required)
    {}

    JsonParam(const std::string &name, const Validator &validator,
              bool required=true)
        : m_name(name)
        , m_required(required)
        , m_validator(validator)
    {}

    JsonParam(const char *name, bool required=true)
        : m_name(name)
        , m_required(required)
    {}

    JsonParam(const char *name, const Validator &validator,
              bool required=true)
        : m_name(name)
        , m_required(required)
        , m_validator(validator)
    {}

    virtual ~JsonParam()
    {}

    const std::string &getName() const
    {
        return m_name;
    }

    virtual bool parse(const rapidjson::Value &elem) = 0;

protected:
    std::string m_name;
    bool m_required;
    Validator m_validator;
};

typedef std::shared_ptr<JsonParam> JsonParamPtr;

template <typename T>
class TypedJsonParam: public JsonParam {
public:
    TypedJsonParam(T &var, const std::string &name, bool required=true)
        : JsonParam(name, required)
        , m_var(var)
    {}

    TypedJsonParam(T &var, const std::string &name, const Validator &validator,
                   bool required=true)
        : JsonParam(name, validator, required)
        , m_var(var)
    {}

    TypedJsonParam(T &var, const char *name, bool required=true)
        : JsonParam(name, required)
        , m_var(var)
    {}

    TypedJsonParam(T &var, const char *name, const Validator &validator,
                   bool required=true)
        : JsonParam(name, validator, required)
        , m_var(var)
    {}

    virtual ~TypedJsonParam()
    {}

    virtual bool parse(const rapidjson::Value &elem);

protected:
    T &m_var;
};

template <typename T>
bool TypedJsonParam<T>::parse(const rapidjson::Value &elem)
{
    if (!JsonParser::parse(m_var, elem, m_name.c_str(), m_required))
    {
        return false;
    }

    if (!m_validator.verify())
    {
        std::string rule = m_validator.getRule(m_name);
        LOG_ERROR("Validation rule failed: %s", rule.c_str());
        return false;
    }

    return true;
}

template <typename T>
JsonParamPtr jsonParam(T &var, const std::string &name, bool required=true)
{
    TypedJsonParam<T> *param = new TypedJsonParam<T>(var, name, required);

    return JsonParamPtr(param);
}

template <typename T>
JsonParamPtr jsonParam(T &var, const std::string &name,
                       const Validator &validator, bool required=true)
{
    TypedJsonParam<T> *param = new TypedJsonParam<T>(var, name, validator,
                                                    required);
    return JsonParamPtr(param);
}

template <typename T>
JsonParamPtr jsonParam(T &var, const char *name, bool required=true)
{
    TypedJsonParam<T> *param = new TypedJsonParam<T>(var, name, required);

    return JsonParamPtr(param);
}

template <typename T>
JsonParamPtr jsonParam(T &var, const char *name,
                       const Validator &validator, bool required=true)
{
    TypedJsonParam<T> *param = new TypedJsonParam<T>(var, name, validator,
                                                    required);
    return JsonParamPtr(param);
}

} // end of namespace bot

#endif

