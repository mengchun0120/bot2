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
        {&m_hp,               "hp",               JSONTYPE_FLOAT},
        {&m_hpRestoreRate,    "hpRestoreRate",    JSONTYPE_FLOAT},
        {&m_armor,            "armor",            JSONTYPE_FLOAT},
        {&m_armorRepairRate,  "armorRepairRate",  JSONTYPE_FLOAT},
        {&m_power,            "power",            JSONTYPE_FLOAT},
        {&m_powerRestoreRate, "powerRestoreRate", JSONTYPE_FLOAT},
        {&m_missileCapacity,  "missileCapacity",  JSONTYPE_INT}
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    return true;
}

} // end of namespace bot

