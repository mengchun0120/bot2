#ifndef INCLUDE_BOT_GAME_OBJECT_TEMPLATE
#define INCLUDE_BOT_GAME_OBJECT_TEMPLATE

#include "gameobj/bot_game_object_type.h"
#include "gameobj/bot_game_object_flag.h"

namespace bot {

class GameObjectTemplate {
public:
    GameObjectTemplate(GameObjectType type);

    virtual ~GameObjectTemplate()
    {}

    GameObjectType getType() const
    {
        return m_type;
    }

    float getCoverBreathX() const
    {
        return m_coverBreathX;
    }

    void setCoverBreathX(float breathX)
    {
        m_coverBreathX = breathX;
    }

    float getCoverBreathY() const
    {
        return m_coverBreathY;
    }

    void setCoverBreathY(float breathY)
    {
        m_coverBreathY = breathY;
    }

    float getCollideBreathX() const
    {
        return m_collideBreathX;
    }

    void setCollideBreathX(float collideBreathX)
    {
        m_collideBreathX = collideBreathX;
    }

    float getCollideBreathY() const
    {
        return m_collideBreathY;
    }

    void setCollideBreathY(float collideBreathY)
    {
        m_collideBreathY = collideBreathY;
    }

    int getFlags() const
    {
        return m_flags;
    }

    void clearFlag(GameObjectFlag flag)
    {
        m_flags &= ~(static_cast<int>(flag));
    }

    void setFlag(GameObjectFlag flag)
    {
        m_flags |= static_cast<int>(flag);
    }

protected:
    GameObjectType m_type;
    float m_coverBreathX;
    float m_coverBreathY;
    float m_collideBreathX;
    float m_collideBreathY;
    int m_flags;
};

} // end of namespace bot

#endif