#ifndef INCLUDE_BOT_TILE_TEMPLATE
#define INCLUDE_BOT_TILE_TEMPLATE

#include <string>
#include "gametemplate/bot_game_object_template.h"
#include "gametemplate/bot_single_unit_template.h"

namespace bot {

class TileTemplate: public GameObjectTemplate, public SingleUnitTemplate {
public:
    TileTemplate();

    virtual ~TileTemplate()
    {}

    bool init(const rapidjson::Value& elem);

    float getHP(int level) const
    {
        return m_hp + level * m_hpPerLevel;
    }

    bool setHP(float hp);

    bool setHPPerLevel(float hpPerLevel);

protected:
    float m_hp;
    float m_hpPerLevel;
};

} // end of namespace bot

#endif
