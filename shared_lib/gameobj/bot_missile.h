#ifndef INCLUDE_BOT_MISSILE
#define INCLUDE_BOT_MISSILE

#include "gametemplate/bot_missile_template.h"
#include "gameobj/bot_game_object.h"

namespace bot {

class Robot;

class Missile: public GameObject {
public:
    Missile();

    virtual ~Missile()
    {}

    bool init(const MissileTemplate* t, const Robot* shooter, float damage,
              float x, float y, float directionX, float directionY);

    virtual void present(Graphics& g);

    virtual void update(float delta, GameScreen& screen);

    const MissileTemplate* getTemplate() const
    {
        return static_cast<const MissileTemplate*>(m_template);
    }

    const Robot* getShooter() const
    {
        return m_shooter;
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

    void explode(GameScreen& gameScreen);

protected:
    bool checkExplosion(GameObject* obj, float left, float bottom,
                        float right, float top);

protected:
    float m_direction[Constants::NUM_FLOATS_PER_POSITION];
    const Robot* m_shooter;
    float m_damage;
};

} // end of namespace bot

#endif
