#ifndef INCLUDE_BOT_JSON_UTILS
#define INCLUDE_BOT_JSON_UTILS

#include <string>
#include <vector>
#include <rapidjson/document.h>
#include "misc/bot_json_param.h"

namespace bot {

bool readJson(rapidjson::Document &doc, const char *fileName);

bool writeJson(const rapidjson::Document &doc, const char *fileName,
               unsigned int indent);

bool parseJson(std::vector<JsonParamPtr> &params,
               const rapidjson::Value &elem);

template <typename PARSER>
bool parseJsonArray(const rapidjson::Value &value,
                    PARSER &parser, const char *name)
{
    if (!value.HasMember(name))
    {
        return false;
    }

    const rapidjson::Value &arr = value[name];
    if (!arr.IsArray())
    {
        return false;
    }

    int len = arr.Capacity();
    for (int i = 0; i < len; ++i)
    {
        if (!parser(arr[i]))
        {
            return false;
        }
    }

    return true;
}

template <typename T, typename PARSER>
bool parseJsonArray(std::vector<T> &vec, const rapidjson::Value &value,
                    PARSER &parser, const char *name)
{
    if (!value.HasMember(name))
    {
        return false;
    }

    const rapidjson::Value &arr = value[name];
    if (!arr.IsArray())
    {
        return false;
    }

    int len = arr.Capacity();
    vec.resize(len);
    for (int i = 0; i < len; ++i)
    {
        if (!parser(vec[i], arr[i]))
        {
            return false;
        }
    }

    return true;
}

} // end of namespace bot

#endif

