#ifndef INCLUDE_BOT_GAME_OBJECT_TEMPLATE
#define INCLUDE_BOT_GAME_OBJECT_TEMPLATE

#include <rapidjson/document.h>
#include "gameobj/bot_game_object_type.h"

namespace bot {

class GameObjectTemplate {
public:
    GameObjectTemplate(GameObjectType type);

    virtual ~GameObjectTemplate()
    {}

    bool init(const rapidjson::Value& elem);

    GameObjectType getType() const
    {
        return m_type;
    }

    float getCoverBreath() const
    {
        return m_coverBreath;
    }

    bool setCoverBreath(float breath);

    float getCollideBreath() const
    {
        return m_collideBreath;
    }

    bool setCollideBreath(float breath);

    int getFlags() const
    {
        return m_flags;
    }

    void clearFlags(int mask)
    {
        m_flags &= ~mask;
    }

    void setFlags(int mask)
    {
        m_flags |= mask;
    }

protected:
    GameObjectType m_type;
    float m_coverBreath;
    float m_collideBreath;
    int m_flags;
};

} // end of namespace bot

#endif
