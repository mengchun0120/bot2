#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "structure/bot_named_map.h"
#include "opengl/bot_texture.h"
#include "geometry/bot_rectangle.h"
#include "gametemplate/bot_single_unit_template.h"

namespace bot {

bool SingleUnitTemplate::init(const NamedMap<Texture>& textureLib,
                              const NamedMap<Rectangle>& rectLib,
                              const rapidjson::Value& elem)
{
    std::string textureName, rectName;
    std::vector<JsonParseParam> params = {
        {
            &textureName,
            "texture",
            JSONTYPE_STRING
        },
        {
            &rectName,
            "rect",
            JSONTYPE_STRING
        }
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
        LOG_ERROR("Failed to find rect %s", rectName.c_str());
        return false;
    }

    return true;
}

} // end of namespace bot

