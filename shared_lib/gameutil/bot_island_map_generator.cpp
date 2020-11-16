#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "structure/bot_named_map.h"
#include "gametemplate/bot_tile_template.h"
#include "gameutil/bot_generated_map.h"
#include "gameutil/bot_island_map_generator.h"

namespace bot {

IslandMapGenerator::IslandMapGenerator()
    : MapGenerator()
    , m_minIslandLenTiles(0)
    , m_maxIslandLenTiles(0)
    , m_minIslandDistSlots(0)
    , m_maxIslandDistSlots(0)
    , m_maxTileWidth(0.0f)
    , m_maxTileHeight(0.0f)
{
}

bool IslandMapGenerator::init(const rapidjson::Value& json,
                              const PlayerTemplate* playerTemplate,
                              const NamedMap<AIRobotTemplate>& robotTemplateLib,
                              const NamedMap<TileTemplate>& tileTemplateLib,
                              int maxRobotCount)
{
    bool ret = MapGenerator::init(json, playerTemplate, robotTemplateLib,
                                  tileTemplateLib, maxRobotCount);
    if (!ret)
    {
        return false;
    }

    std::vector<JsonParamPtr> params = {
        jsonParam(m_minIslandLenTiles, "minIslandLenTiles",
                 gt(m_minIslandLenTiles, 0)),
        jsonParam(m_maxIslandLenTiles, "maxIslandLenTiles",
                  gt(m_maxIslandLenTiles, 0)),
        jsonParam(m_minIslandDistSlots, "minIslandDistSlots",
                  gt(m_minIslandDistSlots, 0)),
        jsonParam(m_maxIslandDistSlots, "maxIslandDistSlots",
                  gt(m_maxIslandDistSlots, 0)),
        jsonParam(m_islandTiles, "islandTiles")
    };

    if (!parseJson(params, json))
    {
        return false;
    }

    int islandTileCount = static_cast<int>(m_islandTiles.size());

    m_islandTileTemplates.resize(islandTileCount);
    for (int i = 0; i < islandTileCount; ++i)
    {
        const TileTemplate* t = tileTemplateLib.search(m_islandTiles[i]);
        if (!t)
        {
            LOG_ERROR("Failed to find tile template %s",
                      m_islandTiles[i].c_str());
            return false;
        }
        m_islandTileTemplates[i] = t;

        float tileWidth = t->getCoverBreath() * 2.0f;
        if (tileWidth > m_maxTileWidth)
        {
            m_maxTileWidth = tileWidth;
        }

        float tileHeight = t->getCoverBreath() * 2.0f;
        if (tileHeight > m_maxTileHeight)
        {
            m_maxTileHeight = tileHeight;
        }
    }

    return true;
}

bool IslandMapGenerator::generate(const char* fileName)
{
    int rowCount = m_rand.get(m_minRowCount, m_maxRowCount + 1);
    int colCount = m_rand.get(m_minColCount, m_maxColCount + 1);

    GeneratedMap map(rowCount, colCount, m_robotSlotSize);

    generateTiles(map);
    MapGenerator::deployRobots(map);

    if (!map.validate())
    {
        LOG_ERROR("Failed to validate map");
        return false;
    }

    if (!map.write(fileName))
    {
        LOG_ERROR("Failed to write to map %s", fileName);
        return false;
    }

    return true;
}

void IslandMapGenerator::generateTiles(GeneratedMap& map)
{
    LOG_INFO("Generating tiles");

    int tileTypeCount = static_cast<int>(m_islandTiles.size());
    int totalSlotsY = map.getSlotRowCount();
    int totalSlotsX = map.getSlotColCount();
    int islandSlotY = m_rand.get(m_minIslandDistSlots, m_maxIslandDistSlots + 1);
    // min and max vertical len of island in terms of slots
    int minIslandLenYSlots =
                 static_cast<int>(ceil(m_minIslandLenTiles *
                                       m_maxTileHeight / m_robotSlotSize));
    int maxIslandLenYSlots =
                 static_cast<int>(ceil(m_maxIslandLenTiles *
                                       m_maxTileHeight / m_robotSlotSize));
    // min and max horizontal len of island in terms of slots
    int minIslandLenXSlots =
                 static_cast<int>(ceil(m_minIslandLenTiles *
                                       m_maxTileWidth / m_robotSlotSize));
    int maxIslandLenXSlots =
                 static_cast<int>(ceil(m_maxIslandLenTiles *
                                       m_maxTileWidth / m_robotSlotSize));

    while (true)
    {
        int maxRowSlots = totalSlotsY - islandSlotY - m_minIslandDistSlots;
        if (maxRowSlots < minIslandLenYSlots)
        {
            break;
        }

        if (maxRowSlots > maxIslandLenYSlots)
        {
            maxRowSlots = maxIslandLenYSlots;
        }

        int islandSlotX = m_rand.get(m_minIslandDistSlots,
                                     m_maxIslandDistSlots + 1);
        while (true)
        {
            int tileTypeIdx = m_rand.get(0, tileTypeCount);
            const TileTemplate* t = m_islandTileTemplates[tileTypeIdx];
            float tileHeight = t->getCoverBreath() * 2.0f;
            float tileWidth = t->getCoverBreath() * 2.0f;

            int maxRowTiles =
                     static_cast<int>(ceil(maxRowSlots *
                                      m_robotSlotSize / tileHeight));
            if (maxRowTiles > m_maxIslandLenTiles)
            {
                maxRowTiles = m_maxIslandLenTiles;
            }

            int maxColSlots = totalSlotsX - islandSlotX - m_minIslandDistSlots;
            if (maxColSlots < minIslandLenXSlots)
            {
                break;
            }

            if (maxColSlots > maxIslandLenXSlots)
            {
                maxColSlots = maxIslandLenXSlots;
            }

            int maxColTiles =
                     static_cast<int>(ceil(maxColSlots *
                                      m_robotSlotSize / tileWidth));
            if (maxColTiles > m_maxIslandLenTiles)
            {
                maxColTiles = m_maxIslandLenTiles;
            }

            int rows = m_rand.get(m_minIslandLenTiles, maxRowTiles + 1);
            int cols = m_rand.get(m_minIslandLenTiles, maxColTiles + 1);

            generateIsland(map, &m_islandTiles[tileTypeIdx], t,
                           islandSlotX, islandSlotY, rows, cols);

            int colSlots = static_cast<int>(ceil(cols * tileWidth /
                                                 m_robotSlotSize));
            islandSlotX += colSlots + m_rand.get(m_minIslandDistSlots,
                                                 m_maxIslandDistSlots + 1);
        }

        islandSlotY += maxRowSlots + m_rand.get(m_minIslandDistSlots,
                                                m_maxIslandDistSlots + 1);
    }

    LOG_INFO("Done generating tiles");
}

void IslandMapGenerator::generateIsland(
                            GeneratedMap& map, const std::string* tileName,
                            const TileTemplate* t, int islandSlotX,
                            int islandSlotY, int rows, int cols)
{
    float tileHeight = t->getCoverBreath() * 2.0f;
    float tileWidth = t->getCoverBreath() * 2.0f;
    float startX = islandSlotX * m_robotSlotSize + t->getCoverBreath();
    float y = islandSlotY * m_robotSlotSize + t->getCoverBreath();

    for (int r = 0; r < rows; ++r)
    {
        float x = startX;
        for (int c = 0; c < cols; ++c)
        {
            map.addTile(tileName, t, x, y);
            x += tileWidth;
        }

        y += tileHeight;
    }
}

} // end of namespace bot
