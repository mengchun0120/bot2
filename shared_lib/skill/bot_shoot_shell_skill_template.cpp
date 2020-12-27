#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "gameobj/bot_shell_template.h"
#include "gameutil/bot_game_lib.h"
#include "skill/bot_shoot_shell_skill_template.h"

namespace bot {

ShootShellSkillTemplate::ShootShellSkillTemplate()
    : AttackSkillTemplate()
    , m_shellTemplate(nullptr)
{
}

bool ShootShellSkillTemplate::init(const rapidjson::Value& elem)
{
    if (!AttackSkillTemplate::init(SKILL_SHOOT_SHELL, elem))
    {
        return false;
    }

    m_flags |= SKILL_FLAG_RANGE;

    std::string shellName;
    std::vector<JsonParamPtr> params = {
        jsonParam(shellName, "shell")
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    const GameLib& lib = GameLib::getInstance();

    const MissileTemplate* t = lib.getMissileTemplate(shellName);
    if (!t)
    {
        LOG_ERROR("Failed to find MissileTemplate %s", shellName.c_str());
        return false;
    }

    if (t->getMissileType() != MISSILE_SHELL)
    {
        LOG_ERROR("Invalid shell %s", shellName.c_str());
        return false;
    }

    m_shellTemplate = static_cast<const ShellTemplate*>(t);

    return true;
}

} // end of namespace bot

