#ifndef INCLUDE_BOT_ROBOT
#define INCLUDE_BOT_ROBOT

#include "misc/bot_time_utils.h"
#include "gameobj/bot_game_object.h"
#include "gameobj/bot_side.h"
#include "gameobj/bot_base.h"
#include "gameobj/bot_weapon.h"
#include "gameobj/bot_mover.h"

namespace bot {

template <typename T> class LinkedList;
class GameObjectItem;
class RobotTemplate;

class Robot : public GameObject {
public:
    Robot();

    virtual ~Robot()
    {}

    bool init(const RobotTemplate* t, Side side,
              int hpLevel, int hpRestoreLevel,
              int armorLevel, int armorRepairLevel,
              int powerLevel, int powerRestoreLevel,
              int weaponLevel, int missileLevel,
              int moverLevel, float x, float y,
              float directionX, float directionY);

    virtual void present();

    void shiftPos(float deltaX, float deltaY);

    void setPos(float x, float y);

    const float* getDirection() const
    {
        return m_direction;
    }

    float getDirectionX() const
    {
        return m_direction[0];
    }

    float getDirectionY() const
    {
        return m_direction[1];
    }

    virtual void setDirection(float directionX, float directionY);

    float getHP() const
    {
        return m_base.getHP();
    }

    bool addHP(float delta);

    void refillHP();

    void setMovingEnabled(bool enabled)
    {
        m_mover.setMoving(enabled);
    }

    bool isMoving() const
    {
        return m_mover.isMoving();
    }

    float getSpeed() const
    {
        return m_mover.getSpeed();
    }

    void setShootingEnabled(bool enabled)
    {
        m_weapon.setFiring(enabled);
    }

    bool isShooting() const
    {
        return m_weapon.isFiring();
    }

    Side getSide() const
    {
        return m_side;
    }

    bool updateMover(float delta, GameScreen& gameScreen);

    bool updateWeapon(GameScreen& gameScreen);

    void updateBase();

    void processCollisions(LinkedList<GameObjectItem>& collideObjs,
                           GameScreen& gameScreen);

    Base& getBase()
    {
        return m_base;
    }

    const Base& getBase() const
    {
        return m_base;
    }

    Weapon& getWeapon()
    {
        return m_weapon;
    }

    const Weapon& getWeapon() const
    {
        return m_weapon;
    }

    Mover& getMover()
    {
        return m_mover;
    }

    const Mover& getMover() const
    {
        return m_mover;
    }

    bool updateMask();

    bool hasDest() const
    {
        return m_hasDest;
    }

    float getDestX() const
    {
        return m_destX;
    }

    float getDestY() const
    {
        return m_destY;
    }

    void setDest(float destX, float destY);

    void unsetDest();

protected:
    Base m_base;
    Weapon m_weapon;
    Mover m_mover;
    float m_direction[Constants::NUM_FLOATS_PER_POSITION];
    Side m_side;
    Color m_mask;
    TimePoint m_deathTime;
    bool m_hasDest;
    float m_destX, m_destY;
};

} // end of namespace bot

#endif
