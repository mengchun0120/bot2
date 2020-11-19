#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "screen/bot_game_screen_config.h"

namespace bot {

GameScreenConfig::GameScreenConfig()
    : m_width(0.0f)
    , m_height(0.0f)
    , m_textWidth(0.0f)
    , m_textHeight(0.0f)
    , m_buttonWidth(0.0f)
    , m_buttonHeight(0.0f)
    , m_buttonSpacing(0.0f)
    , m_textButtonSpacing(0.0f)
{
}

bool GameScreenConfig::load(const std::string& fileName)
{
    rapidjson::Document doc;

    if (!readJson(doc, fileName.c_str()))
    {
        return false;
    }

    if (!doc.IsObject())
    {
        LOG_ERROR("Invalid file format %s", fileName.c_str());
        return false;
    }

    const rapidjson::Value& val = doc.GetObject();
    std::vector<JsonParamPtr> params = {
        jsonParam(m_width, "width", gt(m_width, 0.0f)),
        jsonParam(m_height, "height", gt(m_height, 0.0f)),
        jsonParam(m_textWidth, "textWidth", gt(m_textWidth, 0.0f)),
        jsonParam(m_textHeight, "textHeight", gt(m_textHeight, 0.0f)),
        jsonParam(m_buttonWidth, "buttonWidth", gt(m_buttonWidth, 0.0f)),
        jsonParam(m_buttonHeight, "buttonHeight", gt(m_buttonHeight, 0.0f)),
        jsonParam(m_buttonSpacing, "buttonSpacing", gt(m_buttonSpacing, 0.0f)),
        jsonParam(m_textButtonSpacing, "textButtonSpacing",
                  gt(m_textButtonSpacing, 0.0f)),
        jsonParam(m_buttonTexts, "buttonTexts")
    };

    if (!parseJson(params, val))
    {
        return false;
    }

    return true;
}

} // end of namespace bot

