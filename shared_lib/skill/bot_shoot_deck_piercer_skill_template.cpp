#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "gameobj/bot_deck_piercer_template.h"
#include "gameutil/bot_game_lib.h"
#include "skill/bot_shoot_deck_piercer_skill_template.h"

namespace bot {

ShootDeckPiercerSkillTemplate::ShootDeckPiercerSkillTemplate()
    : AttackSkillTemplate()
    , m_deckPiercerTemplate(nullptr)
    , m_speedMultiplier(1.0f)
{}

bool ShootDeckPiercerSkillTemplate::init(const rapidjson::Value& elem)
{
    if (!AttackSkillTemplate::init(SKILL_SHOOT_DECK_PIERCER, elem))
    {
        return false;
    }

    m_flags |= SKILL_FLAG_RANGE;

    std::string deckPiercerName;
    std::vector<JsonParamPtr> params = {
        jsonParam(deckPiercerName, "deckPiercer")
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    const GameLib& lib = GameLib::getInstance();

    const MissileTemplate* t = lib.getMissileTemplate(deckPiercerName);
    if (!t)
    {
        LOG_ERROR("Failed to find MissileTemplate %s", deckPiercerName.c_str());
        return false;
    }

    if (t->getMissileType() != MISSILE_DECK_PIERCER)
    {
        LOG_ERROR("Invalid shell %s", deckPiercerName.c_str());
        return false;
    }

    m_deckPiercerTemplate = static_cast<const DeckPiercerTemplate*>(t);

    return true;
}

} // end of namespace bot

