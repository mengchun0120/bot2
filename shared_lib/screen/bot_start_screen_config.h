#ifndef INCLUDE_BOT_START_SCREEN_CONFIG
#define INCLUDE_BOT_START_SCREEN_CONFIG

#include <vector>
#include <string>

namespace bot {

class StartScreenConfig {
public:
    StartScreenConfig()
        : m_buttonSpacing(0.0f)
        , m_buttonWidth(0.0f)
        , m_buttonHeight(0.0f)
    {}

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

    const std::vector<std::string>& getButtonTexts() const
    {
        return m_buttonTexts;
    }

private:
    float m_buttonSpacing;
    float m_buttonWidth;
    float m_buttonHeight;
    std::vector<std::string> m_buttonTexts;
};

} // end of namespace bot

#endif
