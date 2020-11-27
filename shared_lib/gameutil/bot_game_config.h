#ifndef INCLUDE_BOT_GAME_CONFIG
#define INCLUDE_BOT_GAME_CONFIG

#include <string>

namespace bot {

class GameConfig {
public:
    GameConfig()
        : m_dissolveTimeMS(0.0f)
    {}

    bool load(const std::string& fileName);

    float getDissolveTimeMS() const
    {
        return m_dissolveTimeMS;
    }

private:
    float m_dissolveTimeMS;
};

} // end of namespace bot

#endif

