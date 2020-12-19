#ifndef INCLUDE_BOT_MISSILE_TEMPLATE
#define INCLUDE_BOT_MISSILE_TEMPLATE

#include <string>
#include "gameobj/bot_game_object_template.h"
#include "gameobj/bot_single_unit_template.h"

namespace bot {

class ParticleEffectTemplate;
class Color;

class MissileTemplate : public GameObjectTemplate, public SingleUnitTemplate {
public:
    MissileTemplate();

    virtual ~MissileTemplate()
    {}

    bool init(const rapidjson::Value& elem);

    float getSpeed() const
    {
        return m_speed;
    }

    bool setSpeed(float speed);

    int getDamage(int level) const
    {
        return m_damage + m_damagePerLevel * (level - 1);
    }

    bool setDamage(int damage);

    bool setDamagePerLevel(float damagePerLevel);

    float getExplosionBreath() const
    {
        return m_explosionBreath;
    }

    bool setExplosionBreath(float explosionBreath);

    const ParticleEffectTemplate* getExplosionTemplate() const
    {
        return m_explosionTemplate;
    }

    void setExplosionTemplate(const ParticleEffectTemplate* explosionTemplate)
    {
        m_explosionTemplate = explosionTemplate;
    }

    const Color* getColor() const
    {
        return m_color;
    }

    void setColor(const Color* color)
    {
        m_color = color;
    }

protected:
    float m_speed;
    float m_damage;
    float m_damagePerLevel;
    float m_explosionBreath;
    const Color* m_color;
    const ParticleEffectTemplate* m_explosionTemplate;
};

} // end of namespace bot

#endif
