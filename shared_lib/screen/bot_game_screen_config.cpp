#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "screen/bot_game_screen_config.h"

namespace bot {

GameScreenConfig::GameScreenConfig()
    : m_msgBoxWidth(0.0f)
    , m_msgBoxHeight(0.0f)
    , m_msgBoxTextWidth(0.0f)
    , m_msgBoxTextHeight(0.0f)
    , m_msgBoxButtonWidth(0.0f)
    , m_msgBoxButtonHeight(0.0f)
    , m_msgBoxButtonSpacing(0.0f)
    , m_msgBoxMsgButtonSpacing(0.0f)
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
    std::vector<JsonParseParam> params = {
        {
            &m_msgBoxWidth,
            "msgBoxWidth",
            JSONTYPE_FLOAT
        },
        {
            &m_msgBoxHeight,
            "msgBoxHeight",
            JSONTYPE_FLOAT
        },
        {
            &m_msgBoxTextWidth,
            "msgBoxTextWidth",
            JSONTYPE_FLOAT
        },
        {
            &m_msgBoxTextHeight,
            "msgBoxTextHeight",
            JSONTYPE_FLOAT
        },
        {
            &m_msgBoxButtonWidth,
            "msgBoxButtonWidth",
            JSONTYPE_FLOAT
        },
        {
            &m_msgBoxButtonHeight,
            "msgBoxButtonHeight",
            JSONTYPE_FLOAT
        },
        {
            &m_msgBoxButtonSpacing,
            "msgBoxButtonSpacing",
            JSONTYPE_FLOAT
        },
        {
            &m_msgBoxMsgButtonSpacing,
            "msgBoxMsgButtonSpacing",
            JSONTYPE_FLOAT
        }
    };

    if (!parseJson(params, val))
    {
        return false;
    }

    if (m_msgBoxWidth <= 0.0f)
    {
        LOG_ERROR("Invalid msgBoxWidth %f", m_msgBoxWidth);
        return false;
    }

    if (m_msgBoxHeight <= 0.0f)
    {
        LOG_ERROR("Invalid msgBoxHeight %f", m_msgBoxHeight);
        return false;
    }
}

} // end of namespace bot

