#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "structure/bot_named_map.h"
#include "opengl/bot_texture.h"
#include "geometry/bot_rectangle.h"
#include "gametemplate/bot_missile_template.h"
#include "gametemplate/bot_weapon_component_template.h"

namespace bot {

bool WeaponComponentTemplate::init(const NamedMap<Texture>& textureLib, const NamedMap<Rectangle>& rectLib,
                                   const NamedMap<MissileTemplate>& missileLib, const rapidjson::Value& elem)
{
    if (!ComponentTemplate::init(textureLib, rectLib, elem))
    {
        return false;
    }

    std::string missileName;
    std::vector<JsonParseParam> params = {
        {&m_damage,    "damage",  JSONTYPE_INT},
        {&missileName, "missile", JSONTYPE_STRING},
        {&m_firePos,   "firePos", JSONTYPE_FLOAT_ARRAY}
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    m_missileTemplate = missileLib.search(missileName);
    if (!m_missileTemplate)
    {
        LOG_ERROR("Failed to find missile %s", missileName.c_str());
        return false;
    }

    return true;
}

} // end of namespace bot

