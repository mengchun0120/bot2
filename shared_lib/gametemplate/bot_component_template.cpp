#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "structure/bot_named_map.h"
#include "opengl/bot_texture.h"
#include "geometry/bot_rectangle.h"
#include "gametemplate/bot_base_component_template.h"
#include "gametemplate/bot_weapon_component_template.h"
#include "gametemplate/bot_mover_component_template.h"

namespace bot {

ComponentTemplate* ComponentTemplate::Parser::create(const std::string& name, const rapidjson::Value& elem)
{
    std::string type;

    if (!parseJson(type, elem, "type"))
    {
        LOG_ERROR("Failed to find type");
        return nullptr;
    }

    if (type == "base")
    {
        BaseComponentTemplate* baseTemplate = new BaseComponentTemplate();

        if (!baseTemplate->init(m_textureLib, m_rectLib, elem))
        {
            delete baseTemplate;
            return nullptr;
        }

        return baseTemplate;
    }
    else if (type == "weapon")
    {
        WeaponComponentTemplate* weaponTemplate = new WeaponComponentTemplate();

        if (!weaponTemplate->init(m_textureLib, m_rectLib, m_missileLib, elem))
        {
            delete weaponTemplate;
            return nullptr;
        }

        return weaponTemplate;
    }
    else if (type == "mover")
    {
        MoverComponentTemplate* moverTemplate = new MoverComponentTemplate();

        if (!moverTemplate->init(m_textureLib, m_rectLib, elem))
        {
            delete moverTemplate;
            return nullptr;
        }

        return moverTemplate;
    }

    return nullptr;
}

bool ComponentTemplate::init(const NamedMap<Texture>& textureLib, const NamedMap<Rectangle>& rectLib,
                             const rapidjson::Value& elem)
{
    std::string textureName, rectName;

    std::vector<JsonParseParam> params = {
        {&textureName, "texture", JSONTYPE_STRING},
        {&rectName,    "rect",    JSONTYPE_STRING}
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

