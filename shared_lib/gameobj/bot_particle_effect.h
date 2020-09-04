#ifndef INCLUDE_BOT_PARTICLE_EFFECT
#define INCLUDE_BOT_PARTICLE_EFFECT

#include "misc/bot_time_utils.h"
#include "gametemplate/bot_particle_effect_template.h"
#include "gameobj/bot_game_object.h"

namespace bot {

class GameScreen;

class ParticleEffect: public GameObject {
public:
    ParticleEffect();

    ParticleEffect(const ParticleEffectTemplate* t);

    virtual ~ParticleEffect()
    {}

    void init(const ParticleEffectTemplate* t, float x, float y);

    const ParticleEffectTemplate* getTemplate() const
    {
        return static_cast<const ParticleEffectTemplate*>(m_template);
    }

    virtual void present(Graphics& g);

    virtual void update(float delta, GameScreen& screen);

protected:
    TimePoint m_startTime;
    float m_duration;
};

} // end of namespace bot

#endif
