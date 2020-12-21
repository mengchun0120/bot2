#ifndef INCLUDE_BOT_SHELL
#define INCLUDE_BOT_SHELL

#include "gameobj/bot_missile_template.h"

namespace bot {

class ParticleEffectTemplate;

class ShellTemplate: public MissileTemplate {
public:
    ShellTemplate();

    virtual ~ShellTemplate()
    {}

    bool init(const rapidjson::Value& elem);

    const ParticleEffectTemplate* getExplodeEffectTemplate() const
    {
        return return m_explodeEffectTemplate;
    }

    float getSpeed() const
    {
        return m_speed;
    }

    float getExplodeBreath() const
    {
        return m_explodeBreath;
    }

protected:
    const ParticleEffectTemplate* m_explodeEffectTemplate;
    float m_speed;
    float m_explodeBreath;
};

} // end of namespace bot

#endif

