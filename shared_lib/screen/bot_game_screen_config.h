#ifndef INCLUDE_BOT_GAME_SCREEN_CONFIG
#define INCLUDE_BOT_GAME_SCREEN_CONFIG

#include <string>

namespace bot {

class GameScreenConfig {
public:
    GameScreenConfig();

    ~GameScreenConfig()
    {}

    bool load(const std::string& fileName);

    float getMsgBoxWidth() const
    {
        return m_msgBoxWidth;
    }

    float getMsgBoxHeight() const
    {
        return m_msgBoxHeight;
    }

    float getMsgBoxTextWidth() const
    {
        return m_msgBoxTextWidth;
    }

    float getMsgBoxTextHeight() const
    {
        return m_msgBoxTextHeight;
    }

    float getMsgBoxButtonWidth() const
    {
        return m_msgBoxButtonWidth;
    }

    float getMsgBoxButtonHeight() const
    {
        return m_msgBoxButtonHeight;
    }

    float getMsgBoxButtonSpacing() const
    {
        return m_msgBoxButtonSpacing;
    }

    float getMsgBoxMsgButtonSpacing() const
    {
        return m_msgBoxMsgButtonSpacing;
    }

private:
    float m_msgBoxWidth, m_msgBoxHeight;
    float m_msgBoxTextWidth, m_msgBoxTextHeight;
    float m_msgBoxButtonWidth, m_msgBoxButtonHeight;
    float m_msgBoxButtonSpacing;
    float m_msgBoxMsgButtonSpacing;
};

} // end of namespace bot

#endif

