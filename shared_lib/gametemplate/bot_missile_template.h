#ifndef INCLUDE_BOT_MISSILE_TEMPLATE
#define INCLUDE_BOT_MISSILE_TEMPLATE

#include <string>
#include "gametemplate/bot_game_object_template.h"
#include "gametemplate/bot_single_unit_template.h"

namespace bot {

class ParticleEffectTemplate;
class Color;

class MissileTemplate : public GameObjectTemplate, public SingleUnitTemplate {
public:
    class Parser {
    public:
        Parser(const NamedMap<Texture>& textureLib, const NamedMap<Rectangle>& rectLib,
               const NamedMap<ParticleEffectTemplate>& particleEffectLib, const NamedMap<Color>& colorLib)
            : m_textureLib(textureLib)
            , m_rectLib(rectLib)
            , m_particleEffectLib(particleEffectLib)
            , m_colorLib(colorLib)
        {}

        ~Parser()
        {}

        MissileTemplate* create(const std::string& name, const rapidjson::Value& elem);

    private:
        const NamedMap<Texture>& m_textureLib;
        const NamedMap<Rectangle>& m_rectLib;
        const NamedMap<ParticleEffectTemplate>& m_particleEffectLib;
        const NamedMap<Color>& m_colorLib;
    };

    MissileTemplate();

    virtual ~MissileTemplate()
    {}

    bool init(const NamedMap<Texture>& textureLib, const NamedMap<Rectangle>& rectLib,
              const NamedMap<ParticleEffectTemplate>& particleEffectLib, const NamedMap<Color>& colorLib,
              const rapidjson::Value& elem);

    float getSpeed() const
    {
        return m_speed;
    }

    bool setSpeed(float speed);

    int getExplosionPower(int level) const
    {
        return m_explosionPower + m_explosionPowerPerLevel * (level - 1);
    }

    bool setExplosionPower(int explosionPower);

    bool setExplosionPowerPerLevel(float powerPerLevel);

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
    float m_explosionPower;
    float m_explosionPowerPerLevel;
    float m_explosionBreath;
    const Color* m_color;
    const ParticleEffectTemplate* m_explosionTemplate;
};

} // end of namespace bot

#endif
