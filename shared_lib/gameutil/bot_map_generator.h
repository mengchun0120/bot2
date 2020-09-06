#ifndef INCLUDE_BOT_MAP_GENERATOR
#define INCLUDE_BOT_MAP_GENERATOR

#include <string>
#include <rapidjson/document.h>
#include "misc/bot_random.h"

namespace bot {

template <typename T> class NamedMap;
class AIRobotTemplate;
class PlayerTemplate;
class TileTemplate;
class GeneratedMap;

class MapGenerator {
public:
    class Parser {
    public:
        Parser(const PlayerTemplate* playerTemplate, const NamedMap<TileTemplate>& tileTemplateLib,
               const NamedMap<AIRobotTemplate>& robotTemplateLib, int maxRobotCount)
            : m_playerTemplate(playerTemplate)
            , m_tileTemplateLib(tileTemplateLib)
            , m_robotTemplateLib(robotTemplateLib)
            , m_maxRobotCount(maxRobotCount)
        {}

        MapGenerator* create(const std::string& name, const rapidjson::Value& elem);

    private:
        const PlayerTemplate* m_playerTemplate;
        const NamedMap<TileTemplate>& m_tileTemplateLib;
        const NamedMap<AIRobotTemplate>& m_robotTemplateLib;
        int m_maxRobotCount;
    };

    MapGenerator();

    virtual ~MapGenerator()
    {}

    virtual bool init(const rapidjson::Value& json, const PlayerTemplate* playerTemplate,
                      const NamedMap<AIRobotTemplate>& aiRobotTemplateLib,
                      const NamedMap<TileTemplate>& tileTemplateLib, int maxRobotCount);

    virtual bool generate(const char* fileName) = 0;

    void setMaxRobotCount(int count)
    {
        m_maxRobotCount = count;
    }

protected:
    virtual int deployRobots(GeneratedMap& map);

protected:
    int m_minRowCount, m_maxRowCount;
    int m_minColCount, m_maxColCount;
    std::vector<std::string> m_robotNames;
    int m_maxRobotCount;
    std::vector<const AIRobotTemplate*> m_robotTemplates;
    float m_robotSlotSize;
    Random m_rand;
};

} // end of namespace bot

#endif
