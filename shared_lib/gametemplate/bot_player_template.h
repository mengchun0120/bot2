#ifndef INCLUDE_BOT_PLAYER_TEMPLATE
#define INCLUDE_BOT_PLAYER_TEMPLATE

#include "gametemplate/bot_robot_template.h"

namespace bot {

template <typename T> class NamedMap;
class Texture;
class Rectangle;
class Color;
class ParticleEffectTemplate;
class MissileTemplate;

class PlayerTemplate: public RobotTemplate {
public:
    PlayerTemplate();

    virtual ~PlayerTemplate();

    bool init(const std::string& fileName,
              const NamedMap<Texture>& textureLib,
              const NamedMap<Rectangle>& rectLib,
              const NamedMap<ParticleEffectTemplate>& particleLib,
              const NamedMap<Color>& colorLib);

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

    int getHPLevel() const
    {
        return m_hpLevel;
    }

    bool setHPLevel(int level);

    int getHPRestoreLevel() const
    {
        return m_hpRestoreLevel;
    }

    bool setHPRestoreLevel(int level);

    int getArmorLevel() const
    {
        return m_armorLevel;
    }

    bool setArmorLevel(int level);

    int getArmorRepairLevel() const
    {
        return m_armorRepairLevel;
    }

    bool setArmorRepairLevel(int level);

    int getPowerLevel() const
    {
        return m_powerLevel;
    }

    bool setPowerLevel(int level);

    int getPowerRestoreLevel() const
    {
        return m_powerRestoreLevel;
    }

    bool setPowerRestoreLevel(int level);

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
    int m_hpLevel;
    int m_hpRestoreLevel;
    int m_armorLevel;
    int m_armorRepairLevel;
    int m_powerLevel;
    int m_powerRestoreLevel;
    int m_missileLevel;
    int m_weaponLevel;
    int m_moverLevel;
};

} // end of namespace bot

#endif
