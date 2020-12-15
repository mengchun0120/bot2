#ifndef INCLUDE_BOT_MISSILE
#define INCLUDE_BOT_MISSILE

#include "structure/bot_linked_list.h"
#include "gametemplate/bot_missile_template.h"
#include "gameobj/bot_game_object.h"
#include "gameobj/bot_side.h"
#include "gameobj/bot_missile_ability.h"

namespace bot {

template <typename T> class LinkedList;
class GameObjectItem;

class Missile: public GameObject {
public:
    Missile();

    virtual ~Missile();

    bool init(const MissileTemplate* t, Side side, float damage,
              float x, float y, float directionX, float directionY,
              MissileAbility ability=MISSILE_ABILITY_NONE);

    virtual void present();

    virtual void update(float delta, GameScreen& screen);

    bool checkCollision(GameScreen& screen);

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

    float getDamage() const
    {
        return m_damage;
    }

    float getSpeed() const
    {
        return getTemplate()->getSpeed();
    }

    float getSpeedX() const
    {
        return getTemplate()->getSpeed() * m_direction[0];
    }

    float getSpeedY() const
    {
        return getTemplate()->getSpeed() * m_direction[1];
    }

    void setDirection(float directionX, float directionY);

    void explode(GameScreen& gameScreen);

    Side getSide() const
    {
        return m_side;
    }

    MissileAbility getAbility() const
    {
        return m_ability;
    }

    virtual void onDealloc();

protected:
    bool checkCollideAbilityNone(GameScreen& screen);

    bool checkCollideAbilityPenetrate(GameScreen& screen);

    bool checkExplosion(GameObject* obj, float left, float bottom,
                        float right, float top);

    bool processPenetrateObjs(LinkedList<GameObjectItem>& collideObjs,
                              GameScreen& screen);

protected:
    float m_direction[Constants::NUM_FLOATS_PER_POSITION];
    Side m_side;
    float m_damage;
    MissileAbility m_ability;
    LinkedList<GameObjectItem> m_penetrateObjs;
};

} // end of namespace bot

#endif
