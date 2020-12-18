#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "gameutil/bot_game_lib.h"
#include "skill/bot_shoot_skill.h"

namespace bot {

ShootSkill::ShootSkill()
    : Skill()
    , m_missile(nullptr)
{
}

bool ShootSkill::init(const rapidjson::Value& elem)
{
    m_type = SKILL_SHOOT;

    if (!Skill::init(elem))
    {
        return false;
    }

    std::string missileName;
    std::vector<JsonParamPtr> params = {
        jsonParam(missileName, "missile")
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    const GameLib& lib = GameLib::getInstance();

    m_missile = lib.getMissileTemplate(missileName);
    if (!m_missile)
    {
        LOG_ERROR("Failed to find missile-template %s", missileName.c_str());
        return false;
    }

    return true;
}

bool ShootSkill::apply(Robot& robot, GameScreen& screen)
{

}

} // end of namespace bot

