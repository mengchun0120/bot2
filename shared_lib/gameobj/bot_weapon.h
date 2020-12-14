#ifndef INCLUDE_BOT_WEAPON
#define INCLUDE_BOT_WEAPON

#include <vector>
#include "misc/bot_time_utils.h"
#include "opengl/bot_color.h"
#include "gameobj/bot_missile_ability.h"
#include "gameobj/bot_fire_point.h"

namespace bot {

class WeaponTemplate;
class MissileTemplate;
class GameScreen;
class Robot;

class Weapon {
public:
    Weapon();

    virtual ~Weapon()
    {}

    bool init(const WeaponTemplate* weaponTemplate, Robot* robot,
              int weaponLevel, int missileLevel);

    bool update(GameScreen& screen);

    void present();

    bool isFiring() const
    {
        return m_firing;
    }

    void setFiring(bool enabled)
    {
        m_firing = enabled;
    }

    void shiftFirePoints(float deltaX, float deltaY);

    void resetFirePoints();

    bool setFireDurationMultiplier(float multiplier);

    bool setDamageMultiplier(float multiplier);

    void setMask(const Color& mask)
    {
        m_mask = mask;
    }

    bool fireMissile(GameScreen& screen,
                     MissileAbility ability=MISSILE_ABILITY_NONE);

private:
    void resetFireDuration();

    void resetDamage();

protected:
    const WeaponTemplate* m_weaponTemplate;
    Robot* m_robot;
    int m_missileLevel;
    bool m_firing;
    std::vector<FirePoint> m_firePoints;
    TimePoint m_lastFireTime;
    float m_normalFireDuration;
    float m_fireDuration;
    float m_fireDurationMultiplier;
    float m_damage;
    float m_damageMultiplier;
    Color m_mask;
};

} // end of namespace bot

#endif

