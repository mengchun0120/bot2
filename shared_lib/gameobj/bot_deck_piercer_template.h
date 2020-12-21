#ifndef INCLUDE_BOT_DECK_PIERCER_TEMPLATE
#define INCLUDE_BOT_DECK_PIERCER_TEMPLATE

#include "opengl/bot_color.h"
#include "gameobj/bot_missile_template.h"

namespace bot {

class ParticleEffectTemplate;

class DeckPiercerTemplate: public MissileTemplate {
public:
    DeckPiercerTemplate();

    virtual ~DeckPiercerTemplate()
    {}

    bool init(const rapidjson::Value& elem);

    const ParticleEffectTemplate* getDeathEffectTemplate() const
    {
        return m_deathEffectTemplate;
    }

    const Color& getPassThroughMask() const
    {
        return m_passThroughMask;
    }

    float getSpeed() const
    {
        return m_speed;
    }

    int getMaxPierceTimes() const
    {
        return m_maxPierceTimes;
    }

    bool getExplodeOnDeath() const
    {
        return m_explodeOnDeath;
    }

protected:
    const ParticleEffectTemplate* m_deathEffectTemplate;
    Color m_passThroughMask;
    float m_speed;
    int m_maxPierceTimes;
    bool m_explodeOnDeath;
};

} // end of namespace bot

#endif

