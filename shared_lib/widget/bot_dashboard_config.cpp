#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "widget/bot_dashboard_config.h"

namespace bot {

bool DashboardConfig::init(const std::string &configFile)
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

    const rapidjson::Value &val = doc.GetObject();

    std::vector<JsonParamPtr> params = {
        jsonParam(m_effectSpacing, "effectSpacing",
                  gt(m_effectSpacing, 0.0f)),
        jsonParam(m_effectRingRadius, "effectRingRadius",
                  gt(m_effectRingRadius, 0.0f)),
        jsonParam(m_effectMargin, "effectMargin", gt(m_effectMargin, 0.0f)),
        jsonParam(m_barSpacing, "barSpacing", gt(m_barSpacing, 0.0f)),
        jsonParam(m_barMargin, "barMargin", gt(m_barMargin, 0.0f)),
        jsonParam(m_statusMargin, "statusMargin", gt(m_statusMargin, 0.0f)),
        jsonParam(m_statusSpacing, "statusSpacing", gt(m_statusSpacing, 0.0f))
    };

    if (!parseJson(params, val))
    {
        return false;
    }

    return true;
}

} // end of namespace bot
