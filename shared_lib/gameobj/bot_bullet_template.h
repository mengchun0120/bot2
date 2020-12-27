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

protected:
    const ParticleEffectTemplate* m_impactEffectTemplate;
};

} // end of namespace bot

#endif

