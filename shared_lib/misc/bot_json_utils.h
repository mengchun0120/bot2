#ifndef INCLUDE_BOT_JSON_UTILS
#define INCLUDE_BOT_JSON_UTILS

#include <string>
#include <vector>
#include <rapidjson/document.h>
#include "misc/bot_log.h"
#include "misc/bot_json_data_type.h"

namespace bot {

struct JsonParseParam {
    void* m_ptr;
    const char* m_name;
    JsonDataType m_type;
};

bool readJson(rapidjson::Document& doc, const char* fileName);

int validateJson(const rapidjson::Value& value, const char* name, JsonDataType type);

bool parseJson(int& r, const rapidjson::Value& value, const char* name);

bool parseJson(float& r, const rapidjson::Value& value, const char* name);

bool parseJson(bool& r, const rapidjson::Value& value, const char* name);

bool parseJson(double& r, const rapidjson::Value& value, const char* name);

bool parseJson(std::string& r, const rapidjson::Value& value, const char* name);

bool parseJson(std::vector<int>& r, const rapidjson::Value& value, const char* name);

bool parseJson(std::vector<float>& r, const rapidjson::Value& value, const char* name);

bool parseJson(std::vector<double>& r, const rapidjson::Value& value, const char* name);

bool parseJson(std::vector<bool>& r, const rapidjson::Value& value, const char* name);

bool parseJson(std::vector<std::string>& r, const rapidjson::Value& value, const char* name);

bool parseJson(std::vector<JsonParseParam>& params, const rapidjson::Value& value);

template <typename PROCESSOR>
bool parseJsonArray(const rapidjson::Value& value, PROCESSOR& processor, const char* name)
{
    if (!value.HasMember(name))
    {
        return false;
    }

    const rapidjson::Value& arr = value[name];
    if (!arr.IsArray())
    {
        return false;
    }

    int len = arr.Capacity();
    for (int i = 0; i < len; ++i)
    {
        if (!processor(arr[i]))
        {
            return false;
        }
    }

    return true;
}

template <typename T>
bool parseVector(std::vector<T>& vec, const char* file)
{
    rapidjson::Document doc;

    if (!readJson(doc, file))
    {
        return false;
    }

    if (!doc.IsArray())
    {
        LOG_ERROR("Invalid format: %s", file);
        return false;
    }

    const rapidjson::Value& arr = doc.GetArray();
    int numObjects = arr.Capacity();

    vec.resize(numObjects);
    for (int i = 0; i < numObjects; ++i)
    {
        const rapidjson::Value& elem = arr[i];
        if (!vec[i].init(elem))
        {
            LOG_ERROR("Failed to parse the %dth object of %s", i, file);
            return false;
        }
    }

    return true;
}

} // end of namespace bot

#endif
