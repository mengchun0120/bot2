#include <cstdio>
#include <rapidjson/filereadstream.h>
#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"

namespace bot {

bool readJson(rapidjson::Document& doc, const char* fileName)
{
    FILE* fp = fopen(fileName, "rb");
    if (!fp)
    {
        LOG_ERROR("Cannot open %s", fileName);
        return false;
    }

    char readBuffer[1000];

    rapidjson::FileReadStream stream(fp, readBuffer, sizeof(readBuffer));
    doc.ParseStream(stream);

    fclose(fp);

    if (doc.HasParseError())
    {
        LOG_ERROR("Failed to parse %s", fileName);
        return false;
    }

    return true;
}

int validateJson(const rapidjson::Value& value, const char* name, JsonDataType type)
{
    typedef bool (rapidjson::Value::*ValidateFunc)() const;

    static ValidateFunc validateFunc[] =
    {
        &rapidjson::Value::IsInt,
        &rapidjson::Value::IsFloat,
        &rapidjson::Value::IsBool,
        &rapidjson::Value::IsDouble,
        &rapidjson::Value::IsString
    };

    if (type < JSONTYPE_INT || type > JSONTYPE_STRING_ARRAY)
    {
        LOG_ERROR("validateJson: invalid type %d", static_cast<int>(type));
        return -1;
    }

    if (!value.HasMember(name))
    {
        LOG_ERROR("validateJson: name %s is missing", name);
        return 0;
    }

    const rapidjson::Value& elem = value[name];

    if (type >= JSONTYPE_INT && type <= JSONTYPE_STRING)
    {
        ValidateFunc func = validateFunc[type];

        if (!(elem.*func)())
        {
            LOG_ERROR("validateJson: %s has wrong data type", name);
            return 0;
        }
    }
    else
    {
        if (!elem.IsArray())
        {
            LOG_ERROR("validateJson: %s has wrong data type", name);
            return 0;
        }

        ValidateFunc func2 = validateFunc[type - JSONTYPE_INT_ARRAY];
        int arraySize = elem.Capacity();

        for (int i = 0; i < arraySize; ++i)
        {
            if (!(elem[i].*func2)())
            {
                LOG_ERROR("validateJson: %s[%d] has wrong type", name, i);
                return 0;
            }
        }
    }

    return 1;
}

bool parseJson(int& r, const rapidjson::Value& value, const char* name)
{
    if (!validateJson(value, name, JSONTYPE_INT))
    {
        return false;
    }

    r = value[name].GetInt();

    return true;
}

bool parseJson(float& r, const rapidjson::Value& value, const char* name)
{
    if (!validateJson(value, name, JSONTYPE_FLOAT))
    {
        return false;
    }

    r = value[name].GetFloat();

    return true;
}

bool parseJson(bool& r, const rapidjson::Value& value, const char* name)
{
    if (!validateJson(value, name, JSONTYPE_BOOL))
    {
        return false;
    }

    r = value[name].GetBool();

    return true;
}

bool parseJson(double& r, const rapidjson::Value& value, const char* name)
{
    if (!validateJson(value, name, JSONTYPE_DOUBLE))
    {
        return false;
    }

    r = value[name].GetDouble();

    return true;
}

bool parseJson(std::string& r, const rapidjson::Value& value, const char* name)
{
    if (!validateJson(value, name, JSONTYPE_STRING))
    {
        return false;
    }

    r = value[name].GetString();

    return true;
}

bool parseJson(std::vector<int>& r, const rapidjson::Value& value, const char* name)
{
    if (1 != validateJson(value, name, JSONTYPE_INT_ARRAY))
    {
        return false;
    }

    const rapidjson::Value& arr = value[name];
    int arrSize = arr.Capacity();

    r.resize(arrSize);
    for (int i = 0; i < arrSize; ++i)
    {
        r[i] = arr[i].GetInt();
    }

    return true;
}

bool parseJson(std::vector<float>& r, const rapidjson::Value& value, const char* name)
{
    if (1 != validateJson(value, name, JSONTYPE_FLOAT_ARRAY))
    {
        return false;
    }

    const rapidjson::Value& arr = value[name];
    int arrSize = arr.Capacity();

    r.resize(arrSize);
    for (int i = 0; i < arrSize; ++i)
    {
        r[i] = arr[i].GetFloat();
    }

    return true;
}

bool parseJson(std::vector<double>& r, const rapidjson::Value& value, const char* name)
{
    if (1 != validateJson(value, name, JSONTYPE_DOUBLE_ARRAY))
    {
        return false;
    }

    const rapidjson::Value& arr = value[name];
    int arrSize = arr.Capacity();

    r.resize(arrSize);
    for (int i = 0; i < arrSize; ++i)
    {
        r[i] = arr[i].GetDouble();
    }

    return true;
}

bool parseJson(std::vector<bool>& r, const rapidjson::Value& value, const char* name)
{
    if (1 != validateJson(value, name, JSONTYPE_BOOL_ARRAY))
    {
        return false;
    }

    const rapidjson::Value& arr = value[name];
    int arrSize = arr.Capacity();

    r.resize(arrSize);
    for (int i = 0; i < arrSize; ++i)
    {
        r[i] = arr[i].GetBool();
    }

    return true;
}

bool parseJson(std::vector<std::string>& r, const rapidjson::Value& value, const char* name)
{
    if (1 != validateJson(value, name, JSONTYPE_STRING_ARRAY))
    {
        return false;
    }

    const rapidjson::Value& arr = value[name];
    int arrSize = arr.Capacity();

    r.resize(arrSize);
    for (int i = 0; i < arrSize; ++i)
    {
        r[i] = arr[i].GetString();
    }

    return true;
}

bool parseJson(std::vector<JsonParseParam>& params, const rapidjson::Value& value)
{
    for (auto it = params.begin(); it != params.end(); ++it)
    {
        switch (it->m_type)
        {
            case JSONTYPE_INT:
            {
                int* r = reinterpret_cast<int*>(it->m_ptr);

                if (!parseJson(*r, value, it->m_name))
                {
                    return false;
                }

                break;
            }
            case JSONTYPE_BOOL:
            {
                bool* r = reinterpret_cast<bool*>(it->m_ptr);

                if (!parseJson(*r, value, it->m_name))
                {
                    return false;
                }

                break;
            }
            case JSONTYPE_FLOAT:
            {
                float* r = reinterpret_cast<float*>(it->m_ptr);

                if (!parseJson(*r, value, it->m_name))
                {
                    return false;
                }

                break;
            }
            case JSONTYPE_DOUBLE:
            {
                double* r = reinterpret_cast<double*>(it->m_ptr);

                if (!parseJson(*r, value, it->m_name))
                {
                    return false;
                }

                break;
            }
            case JSONTYPE_STRING:
            {
                std::string* r = reinterpret_cast<std::string*>(it->m_ptr);

                if (!parseJson(*r, value, it->m_name))
                {
                    return false;
                }

                break;
            }
            case JSONTYPE_INT_ARRAY:
            {
                std::vector<int>* r = reinterpret_cast<std::vector<int>*>(it->m_ptr);

                if (!parseJson(*r, value, it->m_name))
                {
                    return false;
                }

                break;
            }
            case JSONTYPE_FLOAT_ARRAY:
            {
                std::vector<float>* r = reinterpret_cast<std::vector<float>*>(it->m_ptr);

                if (!parseJson(*r, value, it->m_name))
                {
                    return false;
                }

                break;
            }
            case JSONTYPE_BOOL_ARRAY:
            {
                std::vector<bool>* r = reinterpret_cast<std::vector<bool>*>(it->m_ptr);

                if (!parseJson(*r, value, it->m_name))
                {
                    return false;
                }

                break;
            }
            case JSONTYPE_DOUBLE_ARRAY:
            {
                std::vector<double>* r = reinterpret_cast<std::vector<double>*>(it->m_ptr);

                if (!parseJson(*r, value, it->m_name))
                {
                    return false;
                }

                break;
            }
            case JSONTYPE_STRING_ARRAY:
            {
                std::vector<std::string>* r = reinterpret_cast<std::vector<std::string>*>(it->m_ptr);

                if (!parseJson(*r, value, it->m_name))
                {
                    return false;
                }

                break;
            }
            default:
            {
                LOG_ERROR("Invalid json type %d", static_cast<int>(it->m_type));
                return false;
            }
        } // switch (it->m_type)
    }

    return true;
}

} // end of namespace bot
