#include "misc/bot_json_utils.h"
#include "structure/bot_named_map.h"
#include "opengl/bot_texture.h"
#include "geometry/bot_rectangle.h"
#include "gametemplate/bot_mover_component_template.h"

namespace bot {

bool MoverComponentTemplate::init(const NamedMap<Texture>& textureLib, const NamedMap<Rectangle>& rectLib,
                                  const rapidjson::Value& elem)
{
    if (!ComponentTemplate::init(textureLib, rectLib, elem))
    {
        return false;
    }

    std::vector<JsonParseParam> params = {
        {&m_speed, "speed", JSONTYPE_FLOAT}
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    return true;
}

} // end of namespace bot
