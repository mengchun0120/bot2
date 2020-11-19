#ifndef INCLUDE_BOT_GAME_SCREEN_CONFIG
#define INCLUDE_BOT_GAME_SCREEN_CONFIG

#include <string>
#include <vector>

namespace bot {

class GameScreenConfig {
public:
    GameScreenConfig();

    ~GameScreenConfig()
    {}

    bool load(const std::string& fileName);

    float getWidth() const
    {
        return m_width;
    }

    float getHeight() const
    {
        return m_height;
    }

    float getTextWidth() const
    {
        return m_textWidth;
    }

    float getTextHeight() const
    {
        return m_textHeight;
    }

    float getButtonWidth() const
    {
        return m_buttonWidth;
    }

    float getButtonHeight() const
    {
        return m_buttonHeight;
    }

    float getButtonSpacing() const
    {
        return m_buttonSpacing;
    }

    float getTextButtonSpacing() const
    {
        return m_textButtonSpacing;
    }

    const std::vector<std::string>& getButtonTexts() const
    {
        return m_buttonTexts;
    }

private:
    float m_width, m_height;
    float m_textWidth, m_textHeight;
    float m_buttonWidth, m_buttonHeight;
    float m_buttonSpacing;
    float m_textButtonSpacing;
    std::vector<std::string> m_buttonTexts;
};

} // end of namespace bot

#endif

