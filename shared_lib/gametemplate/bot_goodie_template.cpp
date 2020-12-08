#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "structure/bot_named_map.h"
#include "opengl/bot_texture.h"
#include "opengl/bot_color.h"
#include "geometry/bot_rectangle.h"
#include "gametemplate/bot_goodie_template.h"

namespace bot {

GoodieType parseGoodieType(const std::string& typeName)
{
    if (typeName == "gold")
    {
        return GOODIE_GOLD;
    }
    else if (typeName == "health")
    {
        return GOODIE_HEALTH;
    }
    else if (typeName == "indestructable")
    {
        return GOODIE_INDESTRUCTABLE;
    }
    else if (typeName == "double_experience")
    {
        return GOODIE_DOUBLE_EXPERIENCE;
    }
    else if (typeName == "quick_shooter")
    {
        return GOODIE_QUICK_SHOOTER;
    }
    else if (typeName == "quick_mover")
    {
        return GOODIE_QUICK_MOVER;
    }
    else if (typeName == "double_damage")
    {
        return GOODIE_DOUBLE_DAMAGE;
    }

    return GOODIE_UNKNOWN;
}

GoodieTemplate* GoodieTemplate::Parser::create(const std::string& name,
                                               const rapidjson::Value& elem)
{
    GoodieTemplate* t = new GoodieTemplate();
    if (!t->init(name, m_rectLib, m_textureLib, m_colorLib, m_ringLib, elem))
    {
        delete t;
        return nullptr;
    }
    return t;
}

bool GoodieTemplate::init(const std::string& name,
                          const NamedMap<Rectangle>& rectLib,
                          const NamedMap<Texture>& textureLib,
                          const NamedMap<Color>& colorLib,
                          const NamedMap<ProgressRing>& ringLib,
                          const rapidjson::Value& elem)
{
    if (!GameObjectTemplate::init(elem))
    {
        return false;
    }

    std::string textureName, rectName, effectRectName, ringName;
    std::vector<JsonParamPtr> params =
    {
        jsonParam(textureName, "texture"),
        jsonParam(rectName, "rect"),
        jsonParam(effectRectName, "effectRect"),
        jsonParam(ringName, "progressRing"),
        jsonParam(m_duration, "duration"),
        jsonParam(m_weight, "weight", gt(m_weight, 0.0f))
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

    m_effectRect = nullptr;
    if (!effectRectName.empty())
    {
        m_effectRect = rectLib.search(effectRectName);
        if (!m_effectRect)
        {
            LOG_ERROR("Failed to find effect rect %s", effectRectName.c_str());
            return false;
        }
    }

    m_goodieType = parseGoodieType(name);
    if (m_goodieType == GOODIE_UNKNOWN)
    {
        LOG_ERROR("Invalid goodie type: %s", name.c_str());
        return false;
    }

    if (!isInstantaneousGoodie(m_goodieType) && m_duration <= 0.0f)
    {
        LOG_ERROR("Invalid duration %f for goodie %s", m_duration, name);
    }

    m_ring = nullptr;
    if (!ringName.empty())
    {
        m_ring = ringLib.search(ringName);
        if (!m_ring)
        {
            LOG_ERROR("Failed to find progress ring %s", ringName.c_str());
            return false;
        }
    }

    return true;
}

} // end of namespace bot
