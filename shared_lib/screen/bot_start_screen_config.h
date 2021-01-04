#ifndef INCLUDE_BOT_START_SCREEN_CONFIG
#define INCLUDE_BOT_START_SCREEN_CONFIG

#include <vector>
#include <string>
#include "geometry/bot_rectangle.h"

namespace bot {

class StartScreenConfig {
public:
    StartScreenConfig();

    ~StartScreenConfig()
    {}

    bool init(const std::string& configFile);

    float getButtonSpacing() const
    {
        return m_buttonSpacing;
    }

    float getButtonWidth() const
    {
        return m_buttonWidth;
    }

    float getButtonHeight() const
    {
        return m_buttonHeight;
    }

    const Rectangle& getButtonRect() const
    {
        return m_buttonRect;
    }

    const std::vector<std::string>& getButtonTexts() const
    {
        return m_buttonTexts;
    }

private:
    float m_buttonSpacing;
    float m_buttonWidth;
    float m_buttonHeight;
    Rectangle m_buttonRect;
    std::vector<std::string> m_buttonTexts;
};

} // end of namespace bot

#endif

