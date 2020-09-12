#ifndef INCLUDE_BOT_ROBOT_TEMPLATE
#define INCLUDE_BOT_ROBOT_TEMPLATE

#include <vector>
#include <rapidjson/document.h>
#include "misc/bot_constants.h"
#include "gametemplate/bot_game_object_template.h"

namespace bot {

template <typename T> class NamedMap;
class ComponentTemplate;
class BaseComponentTemplate;
class WeaponComponentTemplate;
class MoverComponentTemplate;

class RobotTemplate: public GameObjectTemplate {
public:
    RobotTemplate();

    virtual ~RobotTemplate();

    bool init(const NamedMap<ComponentTemplate>& componentLib, const rapidjson::Value& elem);

    int getBaseCount() const
    {
        return static_cast<int>(m_bases.size());
    }

    const BaseComponentTemplate* getBaseTemplate(int idx) const
    {
        return m_bases[idx];
    }

    int getWeaponCount() const
    {
        return static_cast<int>(m_weapons.size());
    }

    const WeaponComponentTemplate* getWeaponTemplate(int idx) const
    {
        return m_weapons[idx];
    }

    int getMoverCount() const
    {
        return static_cast<int>(m_movers.size());
    }

    const MoverComponentTemplate* getMoverTemplate(int idx) const
    {
        return m_movers[idx];
    }

    float getWeaponPosX() const
    {
        return m_weaponPos[0];
    }

    float getWeaponPosY() const
    {
        return m_weaponPos[1];
    }

    float getMoverPosX() const
    {
        return m_moverPos[0];
    }

    float getMoverPosY() const
    {
        return m_moverPos[1];
    }

protected:
    bool initComponents(const NamedMap<ComponentTemplate>& componentLib, const rapidjson::Value& elem);

    bool initBases(const NamedMap<ComponentTemplate>& componentLib, const std::vector<std::string>& baseNames);

    bool initWeapons(const NamedMap<ComponentTemplate>& componentLib, const std::vector<std::string>& weaponNames);

    bool initMovers(const NamedMap<ComponentTemplate>& componentLib, const std::vector<std::string>& moverNames);

protected:
    std::vector<const BaseComponentTemplate*> m_bases;
    std::vector<const WeaponComponentTemplate*> m_weapons;
    std::vector<const MoverComponentTemplate*> m_movers;
    std::vector<float> m_weaponPos;
    std::vector<float> m_moverPos;
};

} // end of namespace bot

#endif
