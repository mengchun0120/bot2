#ifndef INCLUDE_BOT_PLAYER_TEMPLATE
#define INCLUDE_BOT_PLAYER_TEMPLATE

#include "gametemplate/bot_robot_template.h"

namespace bot {

template <typename T> class NamedMap;
class Texture;
class Rectangle;
class Color;
class ParticleEffectTemplate;

class PlayerTemplate: public RobotTemplate {
public:
    PlayerTemplate();

    virtual ~PlayerTemplate();

    bool init(const std::string& fileName, const NamedMap<Texture>& textureLib,
              const NamedMap<Rectangle>& rectLib, const NamedMap<ParticleEffectTemplate>& particleLib,
              const NamedMap<Color>& color);

    int getGold() const
    {
        return m_gold;
    }

    bool setGold(int gold);

    long long getExperience() const
    {
        return m_experience;
    }

    bool setExperience(float experience);

    int getBaseLevel() const
    {
        return m_baseLevel;
    }

    bool setBaseLevel(int level);

    int getMissileLevel() const
    {
        return m_missileLevel;
    }

    bool setMissileLevel(int level);

    int getWeaponLevel() const
    {
        return m_weaponLevel;
    }

    bool setWeaponLevel(int level);

    int getMoverLevel() const
    {
        return m_moverLevel;
    }

    bool setMoverLevel(int level);

private:
    void configCoverCollideBreath();

protected:
    MissileTemplate* m_missileTemplate;
    int m_gold;
    int m_experience;
    int m_baseLevel;
    int m_missileLevel;
    int m_weaponLevel;
    int m_moverLevel;
};

} // end of namespace bot

#endif
