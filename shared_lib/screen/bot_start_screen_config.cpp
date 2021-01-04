#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "screen/bot_start_screen_config.h"

namespace bot {

StartScreenConfig::StartScreenConfig()
    : m_buttonSpacing(0.0f)
    , m_buttonWidth(0.0f)
    , m_buttonHeight(0.0f)
{}

bool StartScreenConfig::init(const std::string& configFile)
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

    const rapidjson::Value& jsonCfg = doc.GetObject();
    std::vector<JsonParamPtr> params = {
        jsonParam(m_buttonSpacing, "buttonSpacing", gt(m_buttonSpacing, 0.0f)),
        jsonParam(m_buttonWidth, "buttonWidth", gt(m_buttonWidth, 0.0f)),
        jsonParam(m_buttonHeight, "buttonHeight", gt(m_buttonHeight, 0.0f)),
        jsonParam(m_buttonTexts, "buttonTexts")
    };

    if (!parseJson(params, jsonCfg))
    {
        LOG_ERROR("Failed to parse start screen config from %s",
                  configFile.c_str());
        return false;
    }

    if (!m_buttonRect.init(m_buttonWidth, m_buttonHeight, true))
    {
        LOG_ERROR("Failed to initialize buttonRect");
        return false;
    }

    return true;
}

} // end of namespace bot

