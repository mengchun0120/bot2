#ifndef INCLUDE_BOT_MISSILE
#define INCLUDE_BOT_MISSILE

#include "gametemplate/bot_missile_template.h"
#include "gameobj/bot_side.h"
#include "gameobj/bot_game_object.h"

namespace bot {

class Robot;

class Missile: public GameObject {
public:
    Missile();

    Missile(const MissileTemplate* t);

    virtual ~Missile();

    virtual void present(Graphics& g);

    virtual void update(float delta, GameScreen& screen);

    void setTemplate(const MissileTemplate* t)
    {
        m_template = t;
    }

    const MissileTemplate* getTemplate() const
    {
        return static_cast<const MissileTemplate*>(m_template);
    }

    void shiftPos(float deltaX, float deltaY);

    void setPos(float x, float y);

    float getDirectionX() const
    {
        return m_direction[0];
    }

    float getDirectionY() const
    {
        return m_direction[1];
    }

    void setDirection(float directionX, float directionY);

    const Robot* getShooter() const
    {
        return m_shooter;
    }

    void setShooter(const Robot* shooter)
    {
        m_shooter = shooter;
    }

    Side getSide() const
    {
        return m_side;
    }

    void setSide(Side side)
    {
        m_side = side;
    }

    void explode(GameScreen& gameScreen);

    int getExplosionPower() const
    {
        return static_cast<int>(getTemplate()->getExplosionPower() * m_damageMultiplier);
    }

    float getDamageMultiplier() const
    {
        return m_damageMultiplier;
    }

    void setDamageMultiplier(float damageMultiplier)
    {
        m_damageMultiplier = damageMultiplier;
    }

protected:
    bool checkExplosion(GameObject* obj, float left, float bottom, float right, float top, int explosionPower);

protected:
    float m_direction[Constants::NUM_FLOATS_PER_POSITION];
    const Robot* m_shooter;
    Side m_side;
    float m_damageMultiplier;
};

} // end of namespace bot

#endif
