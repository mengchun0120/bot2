#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "gameutil/bot_game_lib.h"
#include "gametemplate/bot_single_unit_template.h"

namespace bot {

bool SingleUnitTemplate::init(const rapidjson::Value& elem)
{
    std::string textureName, rectName;
    std::vector<JsonParamPtr> params = {
        jsonParam(textureName, "texture"),
        jsonParam(rectName, "rect")
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    const GameLib& lib = GameLib::getInstance();

    m_texture = lib.getTexture(textureName);
    if (!m_texture)
    {
        LOG_ERROR("Failed to find texture %s", textureName.c_str());
        return false;
    }

    m_rect = lib.getRect(rectName);
    if (!m_rect)
    {
        LOG_ERROR("Failed to find rect %s", rectName.c_str());
        return false;
    }

    return true;
}

} // end of namespace bot

