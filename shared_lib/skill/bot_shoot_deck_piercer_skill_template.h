#ifndef INCLUDE_SHOOT_DECK_PIERCER_SKILL_TEMPLATE
#define INCLUDE_SHOOT_DECK_PIERCER_SKILL_TEMPLATE

#include "skill/bot_attack_skill_template.h"

namespace bot {

class DeckPiercerTemplate;

class ShootDeckPiercerSkillTemplate: public AttackSkillTemplate {
public:
    ShootDeckPiercerSkillTemplate();

    virtual ~ShootDeckPiercerSkillTemplate()
    {}

    bool init(const rapidjson::Value& elem);

    const DeckPiercerTempalte* getDeckPiercerTemplate() const
    {
        return m_deckPiercerTemplate;
    }

    float getSpeedMultiplier() const
    {
        return m_speedMultiplier;
    }

protected:
    const DeckPiercerTemplate* m_deckPiercerTemplate;
    float m_speedMultiplier;
};


} // end of namespace bo

#endif

