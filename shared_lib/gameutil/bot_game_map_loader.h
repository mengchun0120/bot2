#ifndef INCLUDE_BOT_GAME_MAP_LOADER
#define INCLUDE_BOT_GAME_MAP_LOADER

#include <string>
#include <rapidjson/document.h>
#include "gameutil/bot_game_map.h"
#include "gameutil/bot_game_object_manager.h"

namespace bot {

class Map;
class GameObjectManager;

class GameMapLoader {
public:
    GameMapLoader(GameObjectManager& gameObjManager,
                  float mapPoolFactor)
        : m_gameObjManager(gameObjManager)
        , m_mapPoolFactor(mapPoolFactor)
    {}

    ~GameMapLoader()
    {}

    bool load(GameMap& map, const std::string& mapFile,
              int level, float viewportWidth, float viewportHeight);

private:
    bool initMap(GameMap& map, const rapidjson::Value& mapJson,
                 float viewportWidth, float viewportHeight);

    bool loadTiles(GameMap& map, int level, const rapidjson::Value& mapJson);

    bool addTile(GameMap& map, const std::string& name,
                 int level, float x, float y);

    bool loadRobots(GameMap& map, const rapidjson::Value& mapJson,
                    int level);

    bool addRobot(GameMap& map, const std::string& name, int level,
                  float x, float y,
                  float directionX, float directionY);

    bool loadPlayer(GameMap& map, const rapidjson::Value& mapJson);

private:
    GameObjectManager& m_gameObjManager;
    float m_mapPoolFactor;
};

} // end of namespace bot

#endif
