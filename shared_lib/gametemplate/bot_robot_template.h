#ifndef INCLUDE_BOT_ROBOT_TEMPLATE
#define INCLUDE_BOT_ROBOT_TEMPLATE

#include <vector>
#include <rapidjson/document.h>
#include "misc/bot_constants.h"
#include "gametemplate/bot_move_ability_template.h"
#include "gametemplate/bot_shoot_ability_template.h"
#include "gametemplate/bot_game_object_template.h"

namespace bot {

template <typename T> class NamedMap;
class Texture;
class Rectangle;
class Color;
class MissileTemplate;

class RobotTemplate: public GameObjectTemplate {
public:
    struct ComponentTemplate {
        const Texture* m_texture;
        const Rectangle* m_rect;
        const Color* m_color;
        float m_pos[Constants::NUM_FLOATS_PER_POSITION];
        int m_index;
    };

    static const int MAX_COMPONENTS = 4;

    RobotTemplate();

    virtual ~RobotTemplate();

    bool init(const NamedMap<Texture>& textureLib, const NamedMap<Rectangle>& rectLib,
              const NamedMap<Color>& colorLib, const NamedMap<MissileTemplate>& missileLib,
              const rapidjson::Value& elem);

    int getNumComponents() const
    {
        return static_cast<int>(m_components.size());
    }

    void setNumComponents(int numComponents);

    const ComponentTemplate& getComponent(int componentIdx) const
    {
        return m_components[componentIdx];
    }

    const Texture* getComponentTexture(int componentIdx) const
    {
        return m_components[componentIdx].m_texture;
    }

    void setComponentTexture(int componentIdx, const Texture* texture)
    {
        m_components[componentIdx].m_texture = texture;
    }

    const Rectangle* getComponentRect(int componentIdx) const
    {
        return m_components[componentIdx].m_rect;
    }

    void setComponentRect(int componentIdx, const Rectangle* rect)
    {
        m_components[componentIdx].m_rect = rect;
    }

    const Color* getComponentColor(int componentIdx) const
    {
        return m_components[componentIdx].m_color;
    }

    void setComponentColor(int componentIdx, const Color* color)
    {
        m_components[componentIdx].m_color = color;
    }

    const float* getComponentPos(int componentIdx) const
    {
        return m_components[componentIdx].m_pos;
    }

    float getComponentPosX(int componentIdx) const
    {
        return m_components[componentIdx].m_pos[0];
    }

    void setComponentPosX(int componentIdx, float x)
    {
        m_components[componentIdx].m_pos[0] = x;
    }

    float getComponentPosY(int componentIdx) const
    {
        return m_components[componentIdx].m_pos[1];
    }

    void setComponentPosY(int componentIdx, float y)
    {
        m_components[componentIdx].m_pos[1] = y;
    }

    const MoveAbilityTemplate* getMoveAbilityTemplate() const
    {
        return static_cast<MoveAbilityTemplate*>(m_abilityTemplates[ABILITY_MOVE]);
    }

    void setMoveAbilityTemplate(float speed);

    const ShootAbilityTemplate* getShootAbilityTemplate() const
    {
        return static_cast<ShootAbilityTemplate*>(m_abilityTemplates[ABILITY_SHOOT]);
    }

    void setShootAbilityTemplate(float shootInterval, float shootPosX, float shootPosY,
                                 const MissileTemplate* missileTemplate);

    const ComponentTemplate* getComponentForMoveAbility() const
    {
        return m_attachComponents[ABILITY_MOVE];
    }

    void setComponentForMoveAbility(int componentIdx)
    {
        m_attachComponents[ABILITY_MOVE] = &m_components[componentIdx];
    }

    const ComponentTemplate* getComponentForShootAbility() const
    {
        return m_attachComponents[ABILITY_SHOOT];
    }

    void setComponentForShootAbility(int componentIdx)
    {
        m_attachComponents[ABILITY_SHOOT] = &m_components[componentIdx];
    }

    int getHP() const
    {
        return m_hp;
    }

    void setHP(int hp)
    {
        m_hp = hp;
    }

    float getGoodieSpawnProb() const
    {
        return m_goodieSpawnProb;
    }

    void setGoodieSpawnProb(float goodieSpawnProb)
    {
        m_goodieSpawnProb = goodieSpawnProb;
    }

protected:
    void initComponents();

    void initAbilityTemplates();

    bool parseBaseAttributes(const rapidjson::Value& elem);

    bool parseComponents(const NamedMap<Texture>& textureLib, const NamedMap<Rectangle>& rectLib,
                         const NamedMap<Color>& colorLib, const rapidjson::Value& elem);

    bool parseMoveAbility(const rapidjson::Value& elem);

    bool parseShootAbility(const NamedMap<MissileTemplate>& missileLib, const rapidjson::Value& elem);

protected:
    std::vector<ComponentTemplate> m_components;
    int m_hp;
    AbilityTemplate* m_abilityTemplates[NUM_OF_ABILITIES];
    ComponentTemplate* m_attachComponents[NUM_OF_ABILITIES];
    float m_goodieSpawnProb;
};

} // end of namespace bot

#endif
