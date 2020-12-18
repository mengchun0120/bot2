#ifndef INCLUDE_BOT_MAP_GENERATOR
#define INCLUDE_BOT_MAP_GENERATOR

#include <string>
#include <rapidjson/document.h>
#include "misc/bot_random.h"

namespace bot {

class AIRobotTemplate;
class GeneratedMap;

class MapGenerator {
public:
    static MapGenerator* create(const std::string& name,
                                const rapidjson::Value& elem);
    MapGenerator();

    virtual ~MapGenerator()
    {}

    virtual bool init(const rapidjson::Value& elem);

    virtual bool generate(const char* fileName) = 0;

protected:
    virtual int deployRobots(GeneratedMap& map);

protected:
    int m_minRowCount, m_maxRowCount;
    int m_minColCount, m_maxColCount;
    const PlayerTemplate* m_playerTemplate;
    std::vector<std::string> m_robotNames;
    int m_maxRobotCount;
    std::vector<const AIRobotTemplate*> m_robotTemplates;
    float m_robotSlotSize;
    Random m_rand;
};

} // end of namespace bot

#endif
