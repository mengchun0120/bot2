#ifndef INCLUDE_BOT_PLAYER_DATA
#define INCLUDE_BOT_PLAYER_DATA

#include <string>
#include <vector>

namespace bot {

template <typename T> class NamedMap;
class ComponentTemplate;
class BaseComponentTemplate;
class WeaponComponentTemplate;
class MoverTemplate;
class MissileTemplate;

class PlayerData {
public:
    PlayerData();

    ~PlayerData()
    {}

    bool load(const std::string& fileName, const NamedMap<ComponentTemplate>& componentLib,
              const NamedMap<MissileTemplate>& missileLib);

    float getGoldCount() const
    {
        return m_goldCount;
    }

    long long getExperience() const
    {
        return m_experience;
    }

    float getExtraHP() const
    {
        return m_extraHP;
    }

    float getExtraArmor() const
    {
        return m_extraArmor;
    }

    float getExtraPower() const
    {
        return m_extraPower;
    }

    int getExtraCapacity() const
    {
        return m_extraCapacity;
    }

    const BaseComponentTemplate* getLastBaseTemplate() const
    {
        return m_lastBaseTemplate;
    }

    const WeaponComponentTemplate* getLastWeaponTemplate() const
    {
        return m_lastWeaponTemplate;
    }

    const MoverComponentTemplate* getLastMoverTemplate() const
    {
        return m_lastMoverTemplate;
    }

    const MissileTemplate* getLastMissileTemplate() const
    {
        return m_lastMissileTemplate;
    }

    const std::vector<const BaseComponentTemplate*>& getAvailBaseTemplates() const
    {
        return m_availBaseTemplates;
    }

    const std::vector<const WeaponComponentTemplate*>& getAvailWeaponTemplates() const
    {
        return m_availWeaponTemplates;
    }

    const std::vector<const MoverComponentTemplate*>& getAvailMoverTemplates() const
    {
        return m_availMoverTemplates;
    }

private:
    float m_goldCount;
    long long m_experience;
    float m_extraHP;
    float m_extraArmor;
    float m_extraPower;
    int m_extraCapacity;
    const BaseComponentTemplate* m_lastBaseTemplate;
    const WeaponComponentTemplate* m_lastWeaponTemplate;
    const MoverComponentTemplate* m_lastMoverTemplate;
    const MissileTemplate* m_lastMissileTemplate;
    std::vector<std::string> m_availBaseNames;
    std::vector<std::string> m_availWeaponNames;
    std::vector<std::string> m_availMoverNames;
    std::vector<const BaseComponentTemplate*> m_availBaseTemplates;
    std::vector<const WeaponComponentTemplate*> m_availWeaponTemplates;
    std::vector<const MoverComponentTemplate*> m_availMoverTemplates;
};

} // end of namespace bot

#endif

