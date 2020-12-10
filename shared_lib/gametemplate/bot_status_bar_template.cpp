#include <algorithm>
#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "structure/bot_named_map.h"
#include "opengl/bot_texture.h"
#include "opengl/bot_text_system.h"
#include "gametemplate/bot_status_bar_template.h"

namespace bot {

StatusBarTemplate* StatusBarTemplate::Parser::create(
                                            const std::string& name,
                                            const rapidjson::Value& elem)
{
    StatusBarTemplate* bar = new StatusBarTemplate();

    if (!bar->init(m_textureLib, elem))
    {
        delete bar;
        return nullptr;
    }

    return bar;
}

StatusBarTemplate::StatusBarTemplate()
    : m_texture(nullptr)
    , m_textureTextSpacing(0.0f)
    , m_textLen(0)
    , m_width(0.0f)
    , m_height(0.0f)
    , m_textSize(TEXT_SIZE_SMALL)
{
}

bool StatusBarTemplate::init(const NamedMap<Texture>& textureLib,
                             const rapidjson::Value& elem)
{
    std::string textureName;
    std::vector<float> rectVec;
    std::vector<int> colorVec;
    std::string textSizeName;
    std::vector<JsonParamPtr> params = {
        jsonParam(textureName, "texture"),
        jsonParam(rectVec, "rect"),
        jsonParam(m_textureTextSpacing, "textureTextSpacing",
                  gt(m_textureTextSpacing, 0.0f)),
        jsonParam(colorVec, "textColor"),
        jsonParam(m_textLen, "textLen", gt(m_textLen, 1)),
        jsonParam(textSizeName, "textSize")
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    m_texture = textureLib.search(textureName);
    if (!m_texture)
    {
        LOG_ERROR("Failed to find texture %s", textureName.c_str());
        return false;
    }

    if (!m_rect.init(rectVec, true))
    {
        LOG_ERROR("Failed to initialize rect");
        return false;
    }

    if (!m_textColor.init(colorVec))
    {
        LOG_ERROR("Failed to initialize textColor");
        return false;
    }

    m_textSize = strToTextSize(textSizeName);
    if (!isValidTextSize(m_textSize))
    {
        LOG_ERROR("Invalid textSize %s", textSizeName.c_str());
        return false;
    }

    const TextSystem& textSys = TextSystem::getInstance();

    float maxCharWidth = textSys.getMaxCharWidth(m_textSize);
    float maxTextWidth = maxCharWidth * m_textLen;
    float charHeight = textSys.getCharHeight(m_textSize);

    m_width = m_rect.width() + m_textureTextSpacing + maxTextWidth;
    m_height = std::max(charHeight, m_rect.height());

    return true;
}

} // end of namespace bot

