#ifndef INCLUDE_BOT_GAME_CONFIG
#define INCLUDE_BOT_GAME_CONFIG

#include <string>

namespace bot {

class GameConfig {
public:
    GameConfig()
        : m_dissolveTimeMS(0.0f)
        , m_gameObjItemPoolSize(0)
    {}

    bool load(const std::string &fileName);

    float getDissolveTimeMS() const
    {
        return m_dissolveTimeMS;
    }

    int getGameObjItemPoolSize() const
    {
        return m_gameObjItemPoolSize;
    }

private:
    float m_dissolveTimeMS;
    int m_gameObjItemPoolSize;
};

} // end of namespace bot

#endif

