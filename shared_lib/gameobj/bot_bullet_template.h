#ifndef INCLUDE_BULLET_TEMPLATE
#define INCLUDE_BULLET_TEMPLATE

#include "gameobj/bot_missile_template.h"

namespace bot {

class ParticleEffectTemplate;

class BulletTemplate: public MissileTemplate {
public:
    BulletTemplate();

    virtual ~BulletTemplate();

    bool init(const rapidjson::Value& elem);

    const ParticleEffectTemplate* getImpactEffectTemplate() const
    {
        return m_impactEffectTemplate;
    }

    float getSpeed() const
    {
        return m_speed;
    }

protected:
    const ParticleEffectTemplate* m_impactEffectTemplate;
    float m_speed;
};

} // end of namespace bot

#endif

