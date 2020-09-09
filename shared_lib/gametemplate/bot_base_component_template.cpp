#include "misc/bot_json_utils.h"
#include "structure/bot_named_map.h"
#include "opengl/bot_texture.h"
#include "geometry/bot_rectangle.h"
#include "gametemplate/bot_base_component_template.h"

namespace bot {

bool BaseComponentTemplate::init(const NamedMap<Texture>& textureLib, const NamedMap<Rectangle>& rectLib,
                                 const rapidjson::Value& elem)
{
    if (!ComponentTemplate::init(textureLib, rectLib, elem))
    {
        return false;
    }

    std::vector<JsonParseParam> params = {
        {&m_hp,        "hp",        JSONTYPE_INT},
        {&m_armor,     "armor",     JSONTYPE_INT},
        {&m_power,     "power",     JSONTYPE_INT},
        {&m_weaponPos, "weaponPos", JSONTYPE_FLOAT_ARRAY},
        {&m_moverPos,  "moverPos",  JSONTYPE_FLOAT_ARRAY}
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    return true;
}

} // end of namespace bot

