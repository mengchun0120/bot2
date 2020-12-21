#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "gameutil/bot_game_lib.h"
#include "gameobj/bot_bullet_template.h"

namespace bot {

BulletTemplate::BulletTemplate()
    : MissileTemplate()
    , m_speed(0.0f)
    , m_impactEffectTemplate(nullptr)
{
}

bool BulletTemplate::init(const rapidjson::Value& elem)
{
    if (!MissileTemplate::init(MISSILE_BULLET, elem))
    {
        return false;
    }

    std::string impactEffectName;
    std::vector<JsonParamPtr> params = {
        jsonParam(m_speed, "speed", gt(m_speed, 0.0f)),
        jsonParam(impactEffectName, "impactEffect", false)
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    const GameLib& lib = GameLib::getInstance();

    if (!impactEffectName.empty())
    {
        m_impactEffectTemplate =
                lib.getParticleEffectTemplate(impactEffectName);

        if (!m_impactEffectTemplate)
        {
            LOG_ERROR("Failed to find impact-effect %s",
                      impactEffectName.c_str());
            return false;
        }
    }

    return true;
}

} // end of namespace bot

