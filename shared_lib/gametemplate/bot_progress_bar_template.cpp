#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "structure/bot_named_map.h"
#include "opengl/bot_texture.h"
#include "opengl/bot_color.h"
#include "geometry/bot_rectangle.h"
#include "gametemplate/bot_progress_bar_template.h"

namespace bot {

ProgressBarTemplate* ProgressBarTemplate::Parser::create(
                                        const std::string& name,
                                        const rapidjson::Value& elem)
{
    ProgressBarTemplate* t = new ProgressBarTemplate();

    if (!t->init(m_textureLib, m_colorLib, m_rectLib, elem))
    {
        delete t;
        return nullptr;
    }

    return t;
}

ProgressBarTemplate::ProgressBarTemplate()
    : m_texture(nullptr)
    , m_color(nullptr)
    , m_rect(nullptr)
    , m_barWidth(0.0f)
    , m_barHeight(0.0f)
    , m_slotCount(0)
{}

bool ProgressBarTemplate::init(const NamedMap<Texture>& textureLib,
                               const NamedMap<Color>& colorLib,
                               const NamedMap<Rectangle>& rectLib,
                               const rapidjson::Value& elem)
{
    std::string textureName, rectName;
    std::vector<int> colorVec;
    std::vector<float> startOffsetVec;
    std::vector<JsonParamPtr> params = {
        jsonParam(textureName, "texture"),
        jsonParam(rectName, "rect"),
        jsonParam(colorVec, "color"),
        jsonParam(startOffsetVec, "startOffset"),
        jsonParam(m_barWidth, "barWidth", gt(m_barWidth, 0.0f)),
        jsonParam(m_barHeight, "barHeight", gt(m_barHeight, 0.0f)),
        jsonParam(m_slotCount, "slotCount", gt(m_slotCount, 0))
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

    m_rect = rectLib.search(rectName);
    if (!m_rect)
    {
        LOG_ERROR("Failed to find rectangle %s", rectName.c_str());
        return false;
    }

    return true;
}

} // end of namespace bot

