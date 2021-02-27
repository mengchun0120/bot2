#ifndef INCLUDE_BOT_WEAPON
#define INCLUDE_BOT_WEAPON

#include <vector>
#include "misc/bot_time_utils.h"
#include "opengl/bot_color.h"
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

    bool init(const WeaponTemplate *weaponTemplate, Robot *robot,
              int weaponLevel);

    bool update(GameScreen &screen);

    void present();

    void shiftFirePoints(float deltaX, float deltaY);

    void resetFirePoints();

    bool setDamageMultiplier(float multiplier);

    bool fireMissile(GameScreen &screen, const MissileTemplate *missileTemplate,
                     float damageMultiplier, float speedMultiplier);

protected:
    const WeaponTemplate *m_weaponTemplate;
    Robot *m_robot;
    std::vector<FirePoint> m_firePoints;
    float m_damageMultiplier;
};

} // end of namespace bot

#endif

