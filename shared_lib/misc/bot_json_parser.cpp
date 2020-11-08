#include "misc/bot_log.h"
#include "misc/bot_json_parser.h"

namespace bot {

bool JsonParser::parse(bool& r, const rapidjson::Value& elem,
                       const char* name, bool required)
{
    if (!elem.HasMember(name))
    {
        if (required)
        {
            LOG_ERROR("%s is missing", name);
            return false;
        }
        else
        {
            return true;
        }
    }

    const rapidjson::Value& v = elem["name"];
    if (!v.IsBool())
    {
        LOG_ERROR("%s is not boolean", name);
        return false;
    }

    r = v.GetBool();

    return true;
}

bool JsonParser::parse(int& r, const rapidjson::Value& value,
                       const char* name, bool required)
{
    if (!elem.HasMember(name))
    {
        if (required)
        {
            LOG_ERROR("%s is missing", name);
            return false;
        }
        else
        {
            return true;
        }
    }

    const rapidjson::Value& v = elem["name"];
    if (!v.IsInt())
    {
        LOG_ERROR("%s is not integer", name);
        return false;
    }

    r = v.GetInt();

    return true;
}

bool JsonParser::parse(float& r, const rapidjson::Value& value,
                       const char* name, bool required)
{
    if (!elem.HasMember(name))
    {
        if (required)
        {
            LOG_ERROR("%s is missing", name);
            return false;
        }
        else
        {
            return true;
        }
    }

    const rapidjson::Value& v = elem["name"];
    if (!v.IsFloat())
    {
        LOG_ERROR("%s is not float", name);
        return false;
    }

    r = v.GetFloat();

    return true;
}

bool JsonParser::parse(double& r, const rapidjson::Value& value,
                       const char* name, bool required)
{
    if (!elem.HasMember(name))
    {
        if (required)
        {
            LOG_ERROR("%s is missing", name);
            return false;
        }
        else
        {
            return true;
        }
    }

    const rapidjson::Value& v = elem["name"];
    if (!v.IsDouble())
    {
        LOG_ERROR("%s is not double", name);
        return false;
    }

    r = v.GetDouble();

    return true;
}

bool JsonParser::parse(std::string& r, const rapidjson::Value& value,
                       const char* name, bool required)
{
    if (!elem.HasMember(name))
    {
        if (required)
        {
            LOG_ERROR("%s is missing", name);
            return false;
        }
        else
        {
            return true;
        }
    }

    const rapidjson::Value& v = elem["name"];
    if (!v.IsString())
    {
        LOG_ERROR("%s is not string", name);
        return false;
    }

    r = v.GetString();

    return true;
}

bool JsonParser::parse(std::vector<int>& r, const rapidjson::Value& value,
                       const char* name, bool required)
{
    if (!elem.HasMember(name))
    {
        if (required)
        {
            LOG_ERROR("%s is missing", name);
            return false;
        }
        else
        {
            return true;
        }
    }

    const rapidjson::Value& v = elem["name"];
    if (!v.IsArray())
    {
        LOG_ERROR("%s is not array", name);
        return false;
    }

    int sz = v.GetCapacity();
    if (sz == 0)
    {
        return true;
    }

    r.resize(sz);
    for (int i = 0; i < sz; ++i)
    {
        if (!v[i].IsInt())
        {
            LOG_ERROR("The %dth element of %s is not integer", i, name);
        }

        r[i] = v[i].GetInt();
    }

    return true;
}

bool JsonParser::parse(std::vector<float>& r, const rapidjson::Value& value,
                       const char* name, bool required)
{
    if (!elem.HasMember(name))
    {
        if (required)
        {
            LOG_ERROR("%s is missing", name);
            return false;
        }
        else
        {
            return true;
        }
    }

    const rapidjson::Value& v = elem["name"];
    if (!v.IsArray())
    {
        LOG_ERROR("%s is not array", name);
        return false;
    }

    int sz = v.GetCapacity();
    if (sz == 0)
    {
        return true;
    }

    r.resize(sz);
    for (int i = 0; i < sz; ++i)
    {
        if (!v[i].IsFloat())
        {
            LOG_ERROR("The %dth element of %s is not float", i, name);
        }

        r[i] = v[i].GetFloat();
    }

    return true;
}

bool JsonParser::parse(std::vector<double>& r, const rapidjson::Value& value,
                       const char* name, bool required)
{
    if (!elem.HasMember(name))
    {
        if (required)
        {
            LOG_ERROR("%s is missing", name);
            return false;
        }
        else
        {
            return true;
        }
    }

    const rapidjson::Value& v = elem["name"];
    if (!v.IsArray())
    {
        LOG_ERROR("%s is not array", name);
        return false;
    }

    int sz = v.GetCapacity();
    if (sz == 0)
    {
        return true;
    }

    r.resize(sz);
    for (int i = 0; i < sz; ++i)
    {
        if (!v[i].IsFloat())
        {
            LOG_ERROR("The %dth element of %s is not double", i, name);
        }

        r[i] = v[i].GetDouble();
    }

    return true;
}

bool JsonParser::parse(std::vector<bool>& r, const rapidjson::Value& value,
                       const char* name, bool required)
{
    if (!elem.HasMember(name))
    {
        if (required)
        {
            LOG_ERROR("%s is missing", name);
            return false;
        }
        else
        {
            return true;
        }
    }

    const rapidjson::Value& v = elem["name"];
    if (!v.IsArray())
    {
        LOG_ERROR("%s is not array", name);
        return false;
    }

    int sz = v.GetCapacity();
    if (sz == 0)
    {
        return true;
    }

    r.resize(sz);
    for (int i = 0; i < sz; ++i)
    {
        if (!v[i].IsBool())
        {
            LOG_ERROR("The %dth element of %s is not boolean", i, name);
        }

        r[i] = v[i].GetBool();
    }

    return true;
}

bool JsonParser::parse(std::vector<std::string>& r,
                       const rapidjson::Value& value,
                       const char* name, bool required)
{
    if (!elem.HasMember(name))
    {
        if (required)
        {
            LOG_ERROR("%s is missing", name);
            return false;
        }
        else
        {
            return true;
        }
    }

    const rapidjson::Value& v = elem["name"];
    if (!v.IsArray())
    {
        LOG_ERROR("%s is not array", name);
        return false;
    }

    int sz = v.GetCapacity();
    if (sz == 0)
    {
        return true;
    }

    r.resize(sz);
    for (int i = 0; i < sz; ++i)
    {
        if (!v[i].IsString())
        {
            LOG_ERROR("The %dth element of %s is not string", i, name);
        }

        r[i] = v[i].GetString();
    }

    return true;
}

} // end of namespace bot

