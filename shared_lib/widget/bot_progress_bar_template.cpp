#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "gameutil/bot_game_lib.h"

namespace bot {

ProgressBarTemplate::ProgressBarTemplate()
    : m_texture(nullptr)
    , m_rect(nullptr)
    , m_barWidth(0.0f)
    , m_barHeight(0.0f)
    , m_slotCount(0)
{}

bool ProgressBarTemplate::init(const rapidjson::Value &elem)
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

    const GameLib &lib = GameLib::getInstance();

    m_texture = lib.getTexture(textureName);
    if (!m_texture)
    {
        LOG_ERROR("Failed to find texture %s", textureName.c_str());
        return false;
    }

    m_rect = lib.getRect(rectName);
    if (!m_rect)
    {
        LOG_ERROR("Failed to find rectangle %s", rectName.c_str());
        return false;
    }

    if (!m_color.init(colorVec))
    {
        LOG_ERROR("Invalid color");
        return false;
    }

    if (startOffsetVec.size() != Constants::NUM_FLOATS_PER_POSITION)
    {
        LOG_ERROR("Invalid startOffset");
        return false;
    }

    m_startOffset[0] = startOffsetVec[0];
    m_startOffset[1] = startOffsetVec[1];

    return true;
}

} // end of namespace bot

