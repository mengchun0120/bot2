#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "structure/bot_named_map.h"
#include "opengl/bot_color.h"
#include "geometry/bot_rectangle.h"
#include "widget/bot_message_box_config.h"
#include "app/bot_app.h"

namespace bot {

MessageBoxConfig::MessageBoxConfig()
    : m_boxRect(nullptr)
    , m_buttonRect(nullptr)
    , m_boxFillColor(nullptr)
    , m_boxBorderColor(nullptr)
    , m_buttonSpacing(0.0f)
    , m_buttonY(0.0f)
{
}

bool MessageBoxConfig::init(const std::string& configFile, float viewportWidth, float viewportHeight,
                            const NamedMap<Rectangle>& rectLib, const NamedMap<Color>& colorLib)
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
    std::string boxRectName, buttonRectName, boxFillColorName, boxBorderColorName, textColorName;
    float buttonMarginY;
    std::vector<JsonParseParam> params = {
        {&boxRectName,        "boxRect",        JSONTYPE_STRING},
        {&buttonRectName,     "buttonRect",     JSONTYPE_STRING},
        {&boxFillColorName,   "boxFillColor",   JSONTYPE_STRING},
        {&boxBorderColorName, "boxBorderColor", JSONTYPE_STRING},
        {&textColorName,      "textColor",      JSONTYPE_STRING},
        {&m_buttonTexts,      "buttonTexts",    JSONTYPE_STRING_ARRAY},
        {&m_buttonSpacing,    "buttonSpacing",  JSONTYPE_FLOAT},
        {&m_msgMarginY,       "msgMarginY",     JSONTYPE_FLOAT},
        {&buttonMarginY,      "buttonMarginY",  JSONTYPE_FLOAT}
    };

    if (!parseJson(params, val))
    {
        return false;
    }

    m_boxRect = rectLib.search(boxRectName);
    if (!m_boxRect)
    {
        LOG_ERROR("Failed to find rect %s", boxRectName.c_str());
        return false;
    }

    m_buttonRect = rectLib.search(buttonRectName);
    if (!m_buttonRect)
    {
        LOG_ERROR("Failed to find rect %s", buttonRectName.c_str());
        return false;
    }

    m_boxFillColor = colorLib.search(boxFillColorName);
    if (!m_boxFillColor)
    {
        LOG_ERROR("Failed to find color %s", boxFillColorName.c_str());
        return false;
    }

    m_textColor = colorLib.search(textColorName);
    if (!m_textColor)
    {
        LOG_ERROR("Failed to find color %s", textColorName.c_str());
        return false;
    }

    m_boxBorderColor = colorLib.search(boxBorderColorName);
    if (!m_boxBorderColor)
    {
        LOG_ERROR("Failed to find color %s", boxBorderColorName.c_str());
        return false;
    }

    m_boxPos[0] = viewportWidth / 2.0f;
    m_boxPos[1] = viewportHeight / 2.0f;
    m_boxLeft = m_boxPos[0] - m_boxRect->width() / 2.0f;
    m_boxTop = m_boxPos[1] + m_boxRect->height() / 2.0f;
    m_buttonY = m_boxTop - buttonMarginY;

    return true;
}

} // end of namespace bot
