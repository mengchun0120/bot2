#ifndef INCLUDE_BOT_JSON_PARSE_PARAM
#define INCLUDE_BOT_JSON_PARSE_PARAM

#include <string>
#include <rapidjson/document.h>
#include <misc/bot_param_validator.h>

namespace bot {

class JsonParseParam {
public:
    JsonParseParam(const std::string& name, bool required=true)
        : m_name(name)
    {}

    JsonParseParam(const char* name, bool required=true)
        : m_name(name)
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

template <typename T>
class TypedJsonParseParam: public JsonParseParam {
public:
    typedef bool (Parser*)(T& t, const rapidjson::Value& elem,
                           const std::string& name);

    TypededJsonParseParam(T& t, const std::string& name, Parser parser);

protected:
    T& m_var;
    Parser m_parser;
};

} // end of namespace bot

#endif

