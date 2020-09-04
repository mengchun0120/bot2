#ifndef INCLUDE_BOT_START_SCREEN_CONFIG
#define INCLUDE_BOT_START_SCREEN_CONFIG

#include <vector>
#include <string>

namespace bot {

template <typename T> class NamedMap;
class Rectangle;

class StartScreenConfig {
public:
    StartScreenConfig()
        : m_buttonSpacing(0.0f)
        , m_rect(nullptr)
    {}

    ~StartScreenConfig()
    {}

    bool init(const std::string& configFile, const NamedMap<Rectangle>& rectLib);

    float getButtonSpacing() const
    {
        return m_buttonSpacing;
    }

    const Rectangle* getButtonRect() const
    {
        return m_rect;
    }

    const std::vector<std::string>& getButtonTexts() const
    {
        return m_buttonTexts;
    }

private:
    float m_buttonSpacing;
    const Rectangle* m_rect;
    std::vector<std::string> m_buttonTexts;
};

} // end of namespace bot

#endif
