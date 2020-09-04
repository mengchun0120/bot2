#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "gameobj/bot_tile.h"
#include "gameobj/bot_ai_robot.h"
#include "gameobj/bot_player.h"
#include "gameutil/bot_game_object_manager.h"
#include "gameutil/bot_game_map.h"
#include "gameutil/bot_game_map_loader.h"

namespace bot {

bool GameMapLoader::load(const std::string& file, float viewportWidth, float viewportHeight)
{
    rapidjson::Document doc;
    if (!readJson(doc, file.c_str()))
    {
        LOG_ERROR("Failed to open %s", file.c_str());
        return false;
    }

    if (!doc.IsObject())
    {
        LOG_ERROR("Invalid format %s", file.c_str());
        return false;
    }

    rapidjson::Value mapJson = doc.GetObject();

    if (!initMap(mapJson, viewportWidth, viewportHeight))
    {
        LOG_ERROR("Failed to get map dimension");
        return false;
    }

    if (!loadTiles(mapJson))
    {
        LOG_ERROR("Failed to load tiles");
        return false;
    }

    if (!loadRobots(mapJson))
    {
        LOG_ERROR("Failed to load robots");
        return false;
    }

    if (!loadPlayer(mapJson))
    {
        LOG_ERROR("Failed to load player");
        return false;
    }

    return true;
}

bool GameMapLoader::initMap(const rapidjson::Value& mapJson, float viewportWidth, float viewportHeight)
{
    int numRows, numCols;

    if (!parseJson(numRows, mapJson, "numRows"))
    {
        return false;
    }

    if (!parseJson(numCols, mapJson, "numCols"))
    {
        return false;
    }

    int mapPoolSize = static_cast<int>(numRows * numCols * m_mapPoolFactor);

    m_map.initMap(numRows, numCols, mapPoolSize, viewportWidth, viewportHeight);

    return true;
}

bool GameMapLoader::loadTiles(const rapidjson::Value& mapJson)
{
    std::string name;
    float x, y;
    std::vector<JsonParseParam> params = {
        {&name, "name", JSONTYPE_STRING},
        {&x,    "x",    JSONTYPE_FLOAT},
        {&y,    "y",    JSONTYPE_FLOAT}
    };

    auto parser = [&](const rapidjson::Value& item)->bool
    {
        if (!parseJson(params, item))
        {
            return false;
        }

        return addTile(name, x, y);
    };

    if (!parseJsonArray(mapJson, parser, "tiles"))
    {
        return false;
    }

    return true;
}

bool GameMapLoader::addTile(const std::string& name, float x, float y)
{
    Tile* tile = m_gameObjManager.createTile(name);
    if (!tile)
    {
        return false;
    }
    tile->setPos(x, y);

    m_map.addObject(tile);

    return true;
}

bool GameMapLoader::loadRobots(const rapidjson::Value& mapJson)
{
    std::string name;
    float x, y, directionX, directionY;
    std::vector<JsonParseParam> params = {
        {&name,       "name",       JSONTYPE_STRING},
        {&x,          "x",          JSONTYPE_FLOAT},
        {&y,          "y",          JSONTYPE_FLOAT},
        {&directionX, "directionX", JSONTYPE_FLOAT},
        {&directionY, "directionY", JSONTYPE_FLOAT}
    };

    auto parser = [&](const rapidjson::Value& item)->bool
    {
        if (!parseJson(params, item))
        {
            return false;
        }

        return addRobot(name, x, y, directionX, directionY);
    };

    if (!parseJsonArray(mapJson, parser, "robots"))
    {
        return false;
    }

    return true;
}

bool GameMapLoader::addRobot(const std::string& name, float x, float y, float directionX, float directionY)
{
    AIRobot* robot = m_gameObjManager.createRobot(name, x, y, directionX, directionY, SIDE_AI);
    if (!robot)
    {
        return false;
    }

    m_map.addObject(robot);

    return true;
}

bool GameMapLoader::loadPlayer(const rapidjson::Value& mapJson)
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
    std::vector<JsonParseParam> params = {
        {&x,          "x",          JSONTYPE_FLOAT},
        {&y,          "y",          JSONTYPE_FLOAT},
        {&directionX, "directionX", JSONTYPE_FLOAT},
        {&directionY, "directionY", JSONTYPE_FLOAT}
    };

    if (!parseJson(params, playerJson))
    {
        LOG_ERROR("Failed to parse player");
        return false;
    }

    Player* player = m_gameObjManager.createPlayer(x, y, directionX, directionY);
    m_map.setPlayer(player);

    return true;
}

} // end of namespace bot
