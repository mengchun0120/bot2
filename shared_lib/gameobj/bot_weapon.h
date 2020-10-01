#ifndef INCLUDE_BOT_WEAPON
#define INCLUDE_BOT_WEAPON

#include <vector>
#include "misc/bot_time_utils.h"
#include "gameobj/bot_fire_point.h"

namespace bot {

class WeaponTemplate;
class MissileTemplate;
class GameScreen;
class Graphics;
class Robot;

class Weapon {
public:
    Weapon();

    virtual ~Weapon()
    {}

    bool init(const WeaponTemplate* weaponTemplate, int weaponLevel,
              int missileLevel, float weaponX, float weaponY,
              float directionX, float directionY);

    bool update(GameScreen& screen, Robot& robot);

    void present(Graphics& g, const float* pos, const float* direction);

    bool isFiring() const
    {
        return m_firing;
    }

    void setFiring(bool enabled)
    {
        m_firing = enabled;
    }

    void shiftFirePoints(float deltaX, float deltaY);

    void setFirePoints(float weaponX, float weaponY, float directionX,
                       float directionY);

    bool setFireDurationMultiplier(float multiplier);

    bool setDamageMultiplier(float multiplier);

private:
    void resetFireDuration();

    void resetDamage();

    bool fireMissile(GameScreen& screen, Robot& robot);

protected:
    const WeaponTemplate* m_weaponTemplate;
    const MissileTemplate* m_missileTemplate;
    int m_missileLevel;
    bool m_firing;
    std::vector<FirePoint> m_firePoints;
    TimePoint m_lastFireTime;
    float m_normalFireDuration;
    float m_fireDuration;
    float m_fireDurationMultiplier;
    float m_damage;
    float m_damageMultiplier;
};

} // end of namespace bot

#endif

