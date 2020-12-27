#ifndef INCLUDE_BOT_MISSILE
#define INCLUDE_BOT_MISSILE

#include "gameobj/bot_side.h"
#include "gameobj/bot_game_object.h"
#include "gameobj/bot_missile_template.h"

namespace bot {

class ParticleEffectTemplate;

class Missile: public GameObject {
public:
    Missile();

    virtual ~Missile()
    {}

    bool init(const MissileTemplate* t, Side side, float x, float y,
              float directionX, float directionY, float damage, float speed);

    Side getSide() const
    {
        return m_side;
    }

    MissileType getMissileType() const
    {
        return m_template->getMissileType();
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

    float getDamage() const
    {
        return m_damage;
    }

    float getSpeed() const
    {
        return m_speed;
    }

    float getSpeedX() const
    {
        return m_speed * m_direction[0];
    }

    float getSpeedY() const
    {
        return m_speed * m_direction[1];
    }

    void setDirection(float directionX, float directionY);

protected:
    void explode(GameScreen& screen, float explodeBreath,
                 const ParticleEffectTemplate* explosionTemplate);

    bool checkExplosion(GameObject* obj, float left, float bottom,
                        float right, float top);

protected:
    float m_direction[Constants::NUM_FLOATS_PER_POSITION];
    Side m_side;
    float m_damage;
    float m_speed;
};

} // end of namespace bot

#endif

