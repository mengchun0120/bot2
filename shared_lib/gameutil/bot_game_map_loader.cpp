#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "gameobj/bot_tile.h"
#include "gameobj/bot_ai_robot.h"
#include "gameobj/bot_player.h"
#include "gameutil/bot_game_object_manager.h"
#include "gameutil/bot_game_map.h"
#include "gameutil/bot_game_map_loader.h"

namespace bot {

bool GameMapLoader::load(GameMap& map, const std::string& mapFile,
                         int level, float viewportWidth, float viewportHeight)
{
    if (level < 0)
    {
        LOG_ERROR("Invalid level %d", level);
        return false;
    }

    if (viewportWidth < 0.0f)
    {
        LOG_ERROR("Invalid viewportWidth %f", viewportWidth);
        return false;
    }

    if (viewportHeight < 0.0f)
    {
        LOG_ERROR("Invalid viewportHeight %f", viewportHeight);
        return false;
    }

    rapidjson::Document doc;
    if (!readJson(doc, mapFile.c_str()))
    {
        LOG_ERROR("Failed to open %s", mapFile.c_str());
        return false;
    }

    if (!doc.IsObject())
    {
        LOG_ERROR("Invalid format %s", mapFile.c_str());
        return false;
    }

    rapidjson::Value mapJson = doc.GetObject();

    if (!initMap(map, mapJson, viewportWidth, viewportHeight))
    {
        LOG_ERROR("Failed to get map dimension");
        return false;
    }

    if (!loadTiles(map, level, mapJson))
    {
        LOG_ERROR("Failed to load tiles");
        return false;
    }

    if (!loadRobots(map, mapJson, level))
    {
        LOG_ERROR("Failed to load robots");
        return false;
    }

    if (!loadPlayer(map, mapJson))
    {
        LOG_ERROR("Failed to load player");
        return false;
    }

    LOG_INFO("Done loading map %s", mapFile.c_str());

    return true;
}

bool GameMapLoader::initMap(GameMap& map, const rapidjson::Value& mapJson,
                            float viewportWidth, float viewportHeight)
{
    int numRows, numCols;

    if (!JsonParser::parse(numRows, mapJson, "numRows"))
    {
        return false;
    }

    if (!JsonParser::parse(numCols, mapJson, "numCols"))
    {
        return false;
    }

    if (numRows <= 0)
    {
        LOG_ERROR("Invalid numRows %d", numRows);
        return false;
    }

    if (numCols <= 0)
    {
        LOG_ERROR("Invalid numCols %d", numCols);
        return false;
    }

    int mapPoolSize = static_cast<int>(numRows * numCols * m_mapPoolFactor);

    map.initMap(numRows, numCols, mapPoolSize, viewportWidth, viewportHeight,
                &m_gameObjManager);

    return true;
}

bool GameMapLoader::loadTiles(GameMap& map, int level,
                              const rapidjson::Value& mapJson)
{
    std::string name;
    float x, y;
    std::vector<JsonParamPtr> params = {
        jsonParam(name, "name"),
        jsonParam(x, "x"),
        jsonParam(y, "y")
    };

    auto parser = [&](const rapidjson::Value& item)->bool
    {
        if (!parseJson(params, item))
        {
            return false;
        }

        return addTile(map, name, level, x, y);
    };

    if (!parseJsonArray(mapJson, parser, "tiles"))
    {
        return false;
    }

    return true;
}

bool GameMapLoader::addTile(GameMap& map, const std::string& name, int level,
                            float x, float y)
{
    Tile* tile = m_gameObjManager.createTile(name, level, x, y);
    if (!tile)
    {
        return false;
    }

    if (!map.addObject(tile))
    {
        LOG_ERROR("Failed to add tile to map: name=%s x=%f y=%f",
                  name.c_str(), x, y);

        m_gameObjManager.sendToDeathQueue(tile);

        return false;
    }

    return true;
}

bool GameMapLoader::loadRobots(GameMap& map, const rapidjson::Value& mapJson,
                               int level)
{
    std::string name;
    float x, y, directionX, directionY;
    std::vector<JsonParamPtr> params = {
        jsonParam(name, "name"),
        jsonParam(x, "x"),
        jsonParam(y, "y"),
        jsonParam(directionX, "directionX"),
        jsonParam(directionY, "directionY"),
    };

    auto parser = [&](const rapidjson::Value& item)->bool
    {
        if (!parseJson(params, item))
        {
            return false;
        }

        return addRobot(map, name, level,
                        x, y, directionX, directionY);
    };

    if (!parseJsonArray(mapJson, parser, "robots"))
    {
        return false;
    }

    return true;
}

bool GameMapLoader::addRobot(GameMap& map, const std::string& name,
                             int level, float x, float y,
                             float directionX, float directionY)
{
    AIRobot* robot = m_gameObjManager.createRobot(name, SIDE_AI,
                                                  level, 0,
                                                  level, 0,
                                                  level, level,
                                                  0, level, 0,
                                                  x, y,
                                                  directionX, directionY);
    if (!robot)
    {
        return false;
    }

    if (!map.addObject(robot))
    {
        LOG_ERROR("Failed to add robot to map: name=%s x=%f y=%f",
                  name.c_str(), x, y);

        m_gameObjManager.sendToDeathQueue(robot);

        return false;
    }

    return true;
}

bool GameMapLoader::loadPlayer(GameMap& map, const rapidjson::Value& mapJson)
{
    if (!mapJson.HasMember("player"))
    {
        LOG_ERROR("Config for player doesn't exist");
        return false;
    }

    const rapidjson::Value& playerJson = mapJson["player"];
    if (!playerJson.IsObject())
    {
        LOG_ERROR("Invalid format for player config");
        return false;
    }

    float x, y, directionX, directionY;
    std::vector<JsonParamPtr> params = {
        jsonParam(x, "x"),
        jsonParam(y, "y"),
        jsonParam(directionX, "directionX"),
        jsonParam(directionY, "directionY")
    };

    if (!parseJson(params, playerJson))
    {
        LOG_ERROR("Failed to parse player");
        return false;
    }

    Player* player = m_gameObjManager.createPlayer(x, y,
                                                   directionX, directionY);
    if (!player)
    {
        LOG_ERROR("Failed to create player");
        return false;
    }

    if (!map.setPlayer(player))
    {
        LOG_ERROR("Failed to add player to map");
        return false;
    }

    return true;
}

} // end of namespace bot
