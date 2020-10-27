#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "gameobj/bot_dashboard_config.h"

namespace bot {

bool DashboardConfig::init(const std::string& configFile)
{
    rapidjson::Document doc;

    if (!readJson(doc, configFile.c_str()))
    {
        return false;
    }

    if (!doc.IsObject())
    {
        LOG_ERROR("Invalid file format %s", configFile.c_str());
        return false;
    }

    const rapidjson::Value& val = doc.GetObject();

    std::vector<JsonParseParam> params = {
        {
            &m_effectSpacingX,
            "effectSpacingX",
            JSONTYPE_FLOAT
        },
        {
            &m_effectStartX,
            "effectStartX",
            JSONTYPE_FLOAT
        },
        {
            &m_effectRingRadius,
            "effectRingRadius",
            JSONTYPE_FLOAT
        },
        {
            &m_headerTopMargin,
            "headerTopMargin",
            JSONTYPE_FLOAT
        }
    };

    if (!parseJson(params, val))
    {
        return false;
    }

    return true;
}

} // end of namespace bot
