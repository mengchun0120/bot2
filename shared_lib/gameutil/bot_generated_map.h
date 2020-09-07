#ifndef INCLUDE_BOT_GENERATED_MAP
#define INCLUDE_BOT_GENERATED_MAP

#include <string>
#include <list>
#include <vector>
#include <utility>
#include <cmath>
#include <rapidjson/document.h>

namespace bot {

class TileTemplate;
class AIRobotTemplate;
class PlayerTemplate;
class GameObjectTemplate;

class GeneratedMap {
    struct ObjectItem {
        ObjectItem(const std::string* name, const GameObjectTemplate* t, float x, float y);

        ~ObjectItem()
        {}

        bool outsideRegion(float leftBound, float bottomBound, float rightBound, float topBound) const;

        bool overlap(const ObjectItem& item) const;

        const std::string* m_name;
        const GameObjectTemplate* m_template;
        float m_x, m_y;
        float m_left, m_bottom, m_right, m_top;
    };

    struct TileItem: public ObjectItem {
        TileItem(const std::string* name, const TileTemplate* t, float x, float y);

        ~TileItem()
        {}
    };

    struct RobotItem: public ObjectItem {
        RobotItem(const std::string* name, const AIRobotTemplate* t, float x, float y,
                  float directionX, float directionY);

        ~RobotItem()
        {}

        float m_directionX, m_directionY;
    };

    struct Slot {
        Slot()
            : m_occupied(false)
            , m_x(0.0f)
            , m_y(0.0f)
        {}

        bool m_occupied;
        float m_x, m_y;
    };

public:
    GeneratedMap(int rowCount, int colCount, float slotSize);

    ~GeneratedMap()
    {}

    void setPlayer(const PlayerTemplate* playerTemplate, int row, int col, float directionX, float directionY);

    void addTile(const std::string* name, const TileTemplate* t, float x, float y);

    bool addRobot(const std::string* name, const AIRobotTemplate* t, int row, int col,
                  float directionX, float directionY);

    void getFreeSlots(std::vector<std::pair<int,int>>& freeSlots);

    bool write(const char* fileName);

    int getSlotRowCount() const
    {
        return static_cast<int>(m_slots.size());
    }

    int getSlotColCount() const
    {
        return static_cast<int>(m_slots[0].size());
    }

    bool validate() const;

private:
    void initSlots();

    void toJson(rapidjson::Document& doc);

    int getSlotIndex(float x)
    {
        return static_cast<int>(floor(x / m_slotSize));
    }

    bool validateInRegion() const;

    bool validateOverlap() const;

private:
    std::list<TileItem> m_tiles;
    std::list<RobotItem> m_robots;
    std::vector<std::vector<Slot>> m_slots;
    int m_rowCount, m_colCount;
    float m_mapWidth, m_mapHeight;
    const PlayerTemplate* m_playerTemplate;
    float m_playerX, m_playerY;
    float m_playerDirectionX, m_playerDirectionY;
    float m_slotSize;
};

} // end of namespace bot

#endif
