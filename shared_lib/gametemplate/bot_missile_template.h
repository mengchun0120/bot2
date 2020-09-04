#ifndef INCLUDE_BOT_MISSILE_TEMPLATE
#define INCLUDE_BOT_MISSILE_TEMPLATE

#include <string>
#include <rapidjson/document.h>
#include "gametemplate/bot_game_object_template.h"

namespace bot {

template <typename T> class NamedMap;
class Texture;
class Rectangle;
class ParticleEffectTemplate;
class Color;

class MissileTemplate : public GameObjectTemplate {
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

    void setSpeed(float speed)
    {
        m_speed = speed;
    }

    int getExplosionPower() const
    {
        return m_explosionPower;
    }

    void setExplosionPower(int explosionPower)
    {
        m_explosionPower = explosionPower;
    }

    float getExplosionBreath() const
    {
        return m_explosionBreath;
    }

    void setExplosionBreath(float explosionBreath)
    {
        m_explosionBreath = explosionBreath;
    }

    const ParticleEffectTemplate* getExplosionTemplate() const
    {
        return m_explosionTemplate;
    }

    void setExplosionTemplate(const ParticleEffectTemplate* explosionTemplate)
    {
        m_explosionTemplate = explosionTemplate;
    }

    const Texture* getTexture() const
    {
        return m_texture;
    }

    void setTexture(const Texture* texture)
    {
        m_texture = texture;
    }

    const Rectangle* getRect() const
    {
        return m_rect;
    }

    void setRect(const Rectangle* rect)
    {
        m_rect = rect;
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
    int m_explosionPower;
    float m_explosionBreath;
    const Texture* m_texture;
    const Rectangle* m_rect;
    const Color* m_color;
    const ParticleEffectTemplate* m_explosionTemplate;
};

} // end of namespace bot

#endif
