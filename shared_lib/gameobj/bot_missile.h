#ifndef INCLUDE_BOT_MISSILE
#define INCLUDE_BOT_MISSILE

#include "gametemplate/bot_missile_template.h"
#include "gameobj/bot_game_object.h"
#include "gameobj/bot_side.h"

namespace bot {

class Missile: public GameObject {
public:
    Missile();

    virtual ~Missile()
    {}

    bool init(const MissileTemplate* t, Side side, float damage,
              float x, float y, float directionX, float directionY);

    virtual void present();

    virtual void update(float delta, GameScreen& screen);

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

    void explode(GameScreen& gameScreen);

    Side getSide() const
    {
        return m_side;
    }

protected:
    bool checkExplosion(GameObject* obj, float left, float bottom,
                        float right, float top);

protected:
    float m_direction[Constants::NUM_FLOATS_PER_POSITION];
    Side m_side;
    float m_damage;
};

} // end of namespace bot

#endif
