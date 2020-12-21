#ifndef INCLUDE_BOT_MISSILE_TEMPLATE
#define INCLUDE_BOT_MISSILE_TEMPLATE

#include "gameobj/bot_game_object_template.h"
#include "gameobj/bot_single_unit_template.h"
#include "gameobj/bot_missile_type.h"

namespace bot {

class MissileTemplate : public GameObjectTemplate, public SingleUnitTemplate {
public:
    MissileTemplate();

    virtual ~MissileTemplate()
    {}

    bool init(MissileType missileType, const rapidjson::Value& elem);

    MissileType getMissileType() const
    {
        return m_missileType;
    }

protected:
    MissileType m_missileType;
};

} // end of namespace bot

#endif
