#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "gameobj/bot_shell_template.h"
#include "gameutil/bot_game_lib.h"

namespace bot {

ShellTemplate::ShellTemplate()
    : MissileTemplate()
    , m_explodeEffectTemplate(nullptr)
    , m_speed(0.0f)
    , m_explodeBreath(0.0f)
{
}

bool ShellTemplate::init(const rapidjson::Value& elem)
{
    if (!MissileTemplate::init(MISSILE_SHELL, elem))
    {
        return false;
    }

    std::string explodeEffectName;
    std::vector<JsonParamPtr> params = {
        jsonParam(explodeEffectName, "explodeEffect"),
        jsonParam(m_explodeBreath, "explodeBreath", gt(m_explodeBreath, 0.0f))
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    const GameLib& lib = GameLib::getInstance();

    m_explodeEffectTemplate = lib.getParticleEffectTemplate(explodeEffectName);
    if (!m_explodeEffectTemplate)
    {
        LOG_ERROR("Failed to find explode-effect %s",
                  explodeEffectName.c_str());

        return false;
    }

    return true;
}

} // end of namespace bot

