#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "structure/bot_named_map.h"
#include "geometry/bot_rectangle.h"
#include "screen/bot_start_screen_config.h"

namespace bot {

bool StartScreenConfig::init(const std::string& configFile,
                             const NamedMap<Rectangle>& rectLib)
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
    std::string buttonRectName;
    std::vector<JsonParamPtr> params = {
        jsonParam(m_buttonSpacing, "buttonSpacing", gt(m_buttonSpacing, 0.0f)),
        jsonParam(buttonRectName, "buttonRect"),
        jsonParam(m_buttonTexts, "buttonTexts")
    };

    if (!parseJson(params, jsonCfg))
    {
        LOG_ERROR("Failed to parse start screen config from %s",
                  configFile.c_str());
        return false;
    }

    m_rect = rectLib.search(buttonRectName);
    if (!m_rect)
    {
        LOG_ERROR("Failed to find button %s", buttonRectName.c_str());
        return false;
    }

    return true;
}

} // end of namespace bot
