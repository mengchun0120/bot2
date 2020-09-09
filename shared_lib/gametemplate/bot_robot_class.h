#ifndef INCLUDE_BOT_ROBOT_CLASS
#define INCLUDE_BOT_ROBOT_CLASS

#include <string>
#include <vector>
#include <rapidjson/document.h>

namespace bot {

template <typename T> class NamedMap;
class ComponentTemplate;
class BaseComponentTemplate;
class WeaponComponentTemplate;
class MoverComponentTemplate;

class RobotClass {
public:
    class Parser {
    public:
        Parser(const NamedMap<ComponentTemplate>& componentLib)
            : m_componentLib(componentLib)
        {}

        ~Parser()
        {}

        RobotClass* create(const std::string& name, const rapidjson::Value& elem);

    private:
        const NamedMap<ComponentTemplate>& m_componentLib;
    };

    RobotClass()
        : m_coverBreathX(0.0f)
        , m_coverBreathY(0.0f)
        , m_collideBreathX(0.0f)
        , m_collideBreathY(0.0f)
    {}

    ~RobotClass()
    {}

    bool init(const NamedMap<ComponentTemplate>& componentLib, const rapidjson::Value& elem);

    float getCoverBreathX() const
    {
        return m_coverBreathX;
    }

    float getCoverBreathY() const
    {
        return m_coverBreathY;
    }

    float getCollideBreathX() const
    {
        return m_collideBreathX;
    }

    float getCollideBreathY() const
    {
        return m_collideBreathY;
    }

private:
    bool initBases(const NamedMap<ComponentTemplate>& componentLib, const std::vector<std::string>& baseNames);

    bool initWeapons(const NamedMap<ComponentTemplate>& componentLib, const std::vector<std::string>& weaponNames);

    bool initMovers(const NamedMap<ComponentTemplate>& componentLib, const std::vector<std::string>& moverNames);

protected:
    float m_coverBreathX, m_coverBreathY;
    float m_collideBreathX, m_collideBreathY;
    std::vector<const BaseComponentTemplate*> m_bases;
    std::vector<const WeaponComponentTemplate*> m_weapons;
    std::vector<const MoverComponentTemplate*> m_movers;
    std::vector<float> m_weaponPos;
    std::vector<float> m_moverPos;
};

} // end of namespace bot

#endif

