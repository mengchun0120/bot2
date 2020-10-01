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
    GameMapLoader(GameMap& map, GameObjectManager& gameObjManager, float mapPoolFactor)
        : m_map(map)
        , m_gameObjManager(gameObjManager)
        , m_mapPoolFactor(mapPoolFactor)
    {}

    ~GameMapLoader()
    {}

    bool load(const std::string& file, float viewportWidth, float viewportHeight);

private:
    bool initMap(const rapidjson::Value& mapJson, float viewportWidth,
                 float viewportHeight);

    bool loadTiles(const rapidjson::Value& mapJson);

    bool addTile(const std::string& name, float x, float y);

    bool loadRobots(const rapidjson::Value& mapJson);

    bool addRobot(const std::string& name, const std::string& baseName,
                  const std::string& weaponName, const std::string& moverName,
                  const std::string& missileName, float x, float y,
                  float directionX, float directionY);

    bool loadPlayer(const rapidjson::Value& mapJson);

private:
    GameMap& m_map;
    GameObjectManager& m_gameObjManager;
    float m_mapPoolFactor;
};

} // end of namespace bot

#endif
