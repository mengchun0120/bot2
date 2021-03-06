#ifndef INCLUDE_BOT_ISLAND_MAP_GENERATOR
#define INCLUDE_BOT_ISLAND_MAP_GENERATOR

#include "gameutil/bot_map_generator.h"

namespace bot {

class GeneratedMap;

class IslandMapGenerator: public MapGenerator {
public:
    IslandMapGenerator();

    virtual ~IslandMapGenerator()
    {}

    virtual bool init(const rapidjson::Value &json);

    virtual bool generate(const char *fileName);

private:
    void generateTiles(GeneratedMap &map);

    void generateIsland(GeneratedMap &map, const std::string *tileName,
                        const TileTemplate *t, int islandSlotX, int islandSlotY,
                        int rows, int cols);

private:
    // min and max length of islands in terms of tiles
    int m_minIslandLenTiles, m_maxIslandLenTiles;
    // min and max distance between islands in terms of slots
    int m_minIslandDistSlots, m_maxIslandDistSlots;
    std::vector<std::string> m_islandTiles;
    float m_maxTileWidth, m_maxTileHeight;
    std::vector<const TileTemplate*> m_islandTileTemplates;
};

} // end of namespace bot

#endif
