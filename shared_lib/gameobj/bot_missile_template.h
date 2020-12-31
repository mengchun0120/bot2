#ifndef INCLUDE_BOT_MISSILE_TEMPLATE
#define INCLUDE_BOT_MISSILE_TEMPLATE

#include <rapidjson/document.h>
#include "gameobj/bot_game_object_template.h"
#include "gameobj/bot_single_unit_template.h"
#include "gameobj/bot_missile_type.h"

namespace bot {

class ParticleEffectTemplate;

class MissileTemplate : public GameObjectTemplate, public SingleUnitTemplate {
public:
    static MissileTemplate* create(const std::string& name,
                                   const rapidjson::Value& elem);

    MissileTemplate();

    virtual ~MissileTemplate()
    {}

    bool init(MissileType missileType, const rapidjson::Value& elem);

    MissileType getMissileType() const
    {
        return m_missileType;
    }

    const ParticleEffectTemplate* getImpactEffectTemplate() const
    {
        return m_impactEffectTemplate;
    }

    float getSpeed() const
    {
        return m_speed;
    }

protected:
    MissileType m_missileType;
    const ParticleEffectTemplate* m_impactEffectTemplate;
    float m_speed;
};

} // end of namespace bot

#endif

