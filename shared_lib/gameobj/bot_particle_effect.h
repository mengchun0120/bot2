#ifndef INCLUDE_BOT_PARTICLE_EFFECT
#define INCLUDE_BOT_PARTICLE_EFFECT

#include "misc/bot_time_utils.h"
#include "gameobj/bot_particle_effect_template.h"
#include "gameobj/bot_game_object.h"

namespace bot {

class GameScreen;

class ParticleEffect: public GameObject {
public:
    ParticleEffect();

    ParticleEffect(const ParticleEffectTemplate *t);

    virtual ~ParticleEffect()
    {}

    bool init(const ParticleEffectTemplate *t, float x, float y);

    const ParticleEffectTemplate *getTemplate() const
    {
        return static_cast<const ParticleEffectTemplate*>(m_template);
    }

    virtual void present();

    virtual void update(float delta, GameScreen &screen);

    virtual void onDeath(GameScreen &screen);

protected:
    TimePoint m_startTime;
    float m_duration;
};

} // end of namespace bot

#endif
