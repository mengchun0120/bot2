#include <algorithm>
#include <cmath>
#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "structure/bot_named_map.h"
#include "gametemplate/bot_player_template.h"
#include "gametemplate/bot_ai_robot_template.h"
#include "gameutil/bot_generated_map.h"
#include "gameutil/bot_map_generator.h"
#include "gameutil/bot_island_map_generator.h"

namespace bot {

void randomDirection(Random& rand, float& directionX, float& directionY)
{
    static double TWO_PI = 2.0 * 3.14159265358979323846;
    double theta = rand.get(0.0, TWO_PI);
    directionX = static_cast<float>(cos(theta));
    directionY = static_cast<float>(sin(theta));
}

MapGenerator* MapGenerator::Parser::create(const std::string& name, const rapidjson::Value& elem)
{
    MapGenerator* g = nullptr;

    if ("islandMapGenerator" == name)
    {
        g = new IslandMapGenerator();
        if (!g->init(elem, m_playerTemplate, m_robotTemplateLib, m_tileTemplateLib, m_maxRobotCount))
        {
            delete g;
            g = nullptr;
        }
    }
    else
    {
        LOG_ERROR("Unknown map generator: %s", name.c_str());
    }

    return g;
}

MapGenerator::MapGenerator()
    : m_minRowCount(0)
    , m_maxRowCount(0)
    , m_minColCount(0)
    , m_maxColCount(0)
    , m_playerTemplate(nullptr)
    , m_maxRobotCount(0)
    , m_robotSlotSize(0.0f)
{
}

bool MapGenerator::init(const rapidjson::Value& json, const PlayerTemplate* playerTemplate,
                        const NamedMap<AIRobotTemplate>& aiRobotTemplateLib,
                        const NamedMap<TileTemplate>& tileTemplateLib, int maxRobotCount)
{
    m_playerTemplate = playerTemplate;

    std::vector<JsonParseParam> params = {
        {&m_minRowCount,   "minRowCount",   JSONTYPE_INT},
        {&m_maxRowCount,   "maxRowCount",   JSONTYPE_INT},
        {&m_minColCount,   "minColCount",   JSONTYPE_INT},
        {&m_maxColCount,   "maxColCount",   JSONTYPE_INT},
        {&m_maxRobotCount, "maxRobotCount", JSONTYPE_INT},
        {&m_robotNames,    "robots",        JSONTYPE_STRING_ARRAY},
    };

    if (!parseJson(params, json))
    {
        return false;
    }

    if (maxRobotCount > 0)
    {
        m_maxRobotCount = maxRobotCount;
    }

    int count = static_cast<int>(m_robotNames.size());

    m_robotSlotSize = 2.0f * std::max(playerTemplate->getCoverBreathX(),
                                      playerTemplate->getCoverBreathY());

    m_robotTemplates.resize(count);
    for (int i = 0; i < count; ++i)
    {
        const AIRobotTemplate* t = aiRobotTemplateLib.search(m_robotNames[i]);
        if (!t)
        {
            LOG_ERROR("Failed to find robot template %s", m_robotNames[i].c_str());
            return false;
        }
        m_robotTemplates[i] = t;

        float slotSize = 2.0f * std::max(t->getCoverBreathX(), t->getCoverBreathY());
        if (m_robotSlotSize < slotSize)
        {
            m_robotSlotSize = slotSize;
        }
    }

    return true;
}

int MapGenerator::deployRobots(GeneratedMap& map)
{
    LOG_INFO("Deploying robots");

    std::vector<std::pair<int,int>> freeSlots;
    map.getFreeSlots(freeSlots);

    int freeSlotCount = static_cast<int>(freeSlots.size());
    int lastSlot = freeSlotCount - 1;

    int maxPlayerSlot = std::min(20, freeSlotCount);
    int playerSlot = m_rand.get(0, maxPlayerSlot);
    float playerDirectionX, playerDirectionY;

    randomDirection(m_rand, playerDirectionX, playerDirectionY);
    map.setPlayer(m_playerTemplate, freeSlots[playerSlot].first, freeSlots[playerSlot].second,
                  playerDirectionX, playerDirectionY);

    if (lastSlot != playerSlot)
    {
        std::swap(freeSlots[lastSlot], freeSlots[playerSlot]);
    }
    --freeSlotCount;
    --lastSlot;

    int maxRobotCount = std::min(m_maxRobotCount, freeSlotCount);
    int robotTypeCount = static_cast<int>(m_robotNames.size());
    float directionX, directionY;

    for (int i = 0; i < maxRobotCount; ++i, --freeSlotCount, --lastSlot)
    {
        int robotSlot = m_rand.get(0, freeSlotCount);
        int robotIdx = m_rand.get(0, robotTypeCount);

        randomDirection(m_rand, directionX, directionY);

        map.addRobot(&m_robotNames[robotIdx], m_robotTemplates[robotIdx], freeSlots[robotSlot].first,
                     freeSlots[robotSlot].second, directionX, directionY);

        if (robotSlot != lastSlot)
        {
            std::swap(freeSlots[robotSlot], freeSlots[lastSlot]);
        }
    }

    LOG_INFO("Done deploying robots");

    return 1;
}

} // end of namespace bot
