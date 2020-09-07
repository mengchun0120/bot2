#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>
#include <fstream>
#include "misc/bot_log.h"
#include "misc/bot_math_utils.h"
#include "gametemplate/bot_tile_template.h"
#include "gametemplate/bot_ai_robot_template.h"
#include "gameutil/bot_generated_map.h"
#include "gameutil/bot_game_map.h"

namespace bot {

const float THRESHOLD = 0.01;

GeneratedMap::ObjectItem::ObjectItem(const std::string* name, const GameObjectTemplate* t,
                                     float x, float y)
    : m_name(name)
    , m_template(t)
    , m_x(x)
    , m_y(y)
{
    m_left = x - t->getCoverBreathX();
    m_right = x + t->getCoverBreathX();
    m_bottom = y - t->getCoverBreathY();
    m_top = y + t->getCoverBreathY();
}

bool GeneratedMap::ObjectItem::outsideRegion(float leftBound, float bottomBound, float rightBound, float topBound) const
{
    return compare(m_left, leftBound, THRESHOLD) < 0 ||
           compare(m_right, rightBound, THRESHOLD) > 0 ||
           compare(m_bottom, bottomBound, THRESHOLD) < 0 ||
           compare(m_top, topBound, THRESHOLD) > 0;
}

bool GeneratedMap::ObjectItem::overlap(const ObjectItem& item) const
{
    return compare(m_left, item.m_right, THRESHOLD) < 0 &&
           compare(m_right, item.m_left, THRESHOLD) > 0 &&
           compare(m_bottom, item.m_top, THRESHOLD) < 0 &&
           compare(m_top, item.m_bottom, THRESHOLD) > 0;
}

GeneratedMap::TileItem::TileItem(const std::string* name, const TileTemplate* t, float x, float y)
    : ObjectItem(name, t, x, y)
{
}

GeneratedMap::RobotItem::RobotItem(const std::string* name, const AIRobotTemplate* t, float x, float y,
                                   float directionX, float directionY)
    : ObjectItem(name, t, x, y)
{
    m_directionX = directionX;
    m_directionY = directionY;
}


GeneratedMap::GeneratedMap(int rowCount, int colCount, float slotSize)
    : m_rowCount(rowCount)
    , m_colCount(colCount)
    , m_mapWidth(m_colCount* GameMap::GRID_BREATH)
    , m_mapHeight(m_rowCount* GameMap::GRID_BREATH)
    , m_playerTemplate(nullptr)
    , m_playerX(0.0f)
    , m_playerY(0.0f)
    , m_playerDirectionX(0.0f)
    , m_playerDirectionY(0.0f)
    , m_slotSize(slotSize)
{
    LOG_INFO("mapWidth=%f mapHeight=%f", m_mapWidth, m_mapHeight);
    initSlots();
}

void GeneratedMap::initSlots()
{
    int slotRowCount = static_cast<int>(floor(m_mapHeight / m_slotSize));
    int slotColCount = static_cast<int>(floor(m_mapWidth / m_slotSize));

    LOG_INFO("slotRowCount=%d slotColCount=%d", slotRowCount, slotColCount);

    float y = m_slotSize / 2.0f;

    m_slots.resize(slotRowCount);
    for (int r = 0; r < slotRowCount; ++r)
    {
        std::vector<Slot>& row = m_slots[r];
        row.resize(slotColCount);

        float x = m_slotSize / 2.0f;
        for (int c = 0; c < slotColCount; ++c)
        {
            row[c].m_x = x;
            row[c].m_y = y;
            x += m_slotSize;
        }

        y += m_slotSize;
    }
}

void GeneratedMap::setPlayer(const PlayerTemplate* playerTemplate, int row, int col, float directionX, float directionY)
{
    Slot& slot = m_slots[row][col];
    m_playerTemplate = playerTemplate;
    m_playerX = slot.m_x;
    m_playerY = slot.m_y;
    slot.m_occupied = true;
    m_playerDirectionX = directionX;
    m_playerDirectionY = directionY;
}

void GeneratedMap::addTile(const std::string* name, const TileTemplate* t, float x, float y)
{
    m_tiles.emplace_back(name, t, x, y);

    int maxRowIdx = static_cast<int>(m_slots.size()) - 1;
    int maxColIdx = static_cast<int>(m_slots[0].size()) - 1;
    int left = clamp(getSlotIndex(x - t->getCoverBreathX()), 0, maxColIdx);
    int right = clamp(getSlotIndex(x + t->getCoverBreathX()), 0, maxColIdx);
    int bottom = clamp(getSlotIndex(y - t->getCoverBreathY()), 0, maxRowIdx);
    int top = clamp(getSlotIndex(y + t->getCoverBreathY()), 0, maxRowIdx);

    for (int r = bottom; r <= top; ++r)
    {
        auto& row = m_slots[r];
        for (int c = left; c <= right; ++c)
        {
            row[c].m_occupied = true;
        }
    }
}

bool GeneratedMap::addRobot(const std::string* name, const AIRobotTemplate* t, int row, int col,
                            float directionX, float directionY)
{
    Slot& slot = m_slots[row][col];
    if (slot.m_occupied)
    {
        return false;
    }

    slot.m_occupied = true;
    m_robots.emplace_back(name, t, slot.m_x, slot.m_y, directionX, directionY);

    return true;
}

bool GeneratedMap::write(const char* fileName)
{
    using namespace rapidjson;

    LOG_INFO("Writing map to %s", fileName);

    Document doc;
    toJson(doc);

    std::ofstream ofs(fileName);
    if (!ofs.good())
    {
        return false;
    }

    OStreamWrapper osw(ofs);
    Writer<OStreamWrapper> writer(osw);

    doc.Accept(writer);

    ofs.close();

    LOG_INFO("Done writing map to %s", fileName);

    return true;
}

void GeneratedMap::toJson(rapidjson::Document& doc)
{
    using namespace rapidjson;

    Document::AllocatorType& allocator = doc.GetAllocator();

    doc.SetObject();
    doc.AddMember("numRows", m_rowCount, allocator);
    doc.AddMember("numCols", m_colCount, allocator);

    Value player(kObjectType);
    player.AddMember("x", m_playerX, allocator);
    player.AddMember("y", m_playerY, allocator);
    player.AddMember("directionX", m_playerDirectionX, allocator);
    player.AddMember("directionY", m_playerDirectionY, allocator);
    doc.AddMember("player", player, allocator);

    Value tiles(kArrayType);
    tiles.Reserve(m_tiles.size(), allocator);
    int i = 0;
    for (auto& t : m_tiles)
    {
        Value tile(kObjectType);
        tile.AddMember("name", StringRef(t.m_name->c_str()), allocator);
        tile.AddMember("x", t.m_x, allocator);
        tile.AddMember("y", t.m_y, allocator);
        tiles.PushBack(tile, allocator);
        ++i;
    }
    doc.AddMember("tiles", tiles, allocator);

    Value robots(kArrayType);
    robots.Reserve(m_robots.size(), allocator);
    for (auto& t : m_robots)
    {
        Value robot(kObjectType);
        robot.AddMember("name", StringRef(t.m_name->c_str()), allocator);
        robot.AddMember("x", t.m_x, allocator);
        robot.AddMember("y", t.m_y, allocator);
        robot.AddMember("directionX", t.m_directionX, allocator);
        robot.AddMember("directionY", t.m_directionY, allocator);
        robots.PushBack(robot, allocator);
    }
    doc.AddMember("robots", robots, allocator);
}

void GeneratedMap::getFreeSlots(std::vector<std::pair<int,int>>& freeSlots)
{
    int freeSlotCount = 0;

    for (auto& row: m_slots)
    {
        for (auto& slot: row)
        {
            if (!slot.m_occupied)
            {
                ++freeSlotCount;
            }
        }
    }

    int i = 0;
    int rowCount = static_cast<int>(m_slots.size());
    int colCount = static_cast<int>(m_slots[0].size());

    freeSlots.resize(freeSlotCount);
    for (int r = 0; r < rowCount; ++r)
    {
        auto& row = m_slots[r];
        for (int c = 0; c < colCount; ++c)
        {
            if (!row[c].m_occupied)
            {
                freeSlots[i].first = r;
                freeSlots[i].second = c;
                ++i;
            }
        }
    }
}

bool GeneratedMap::validate() const
{
    return validateInRegion() && validateOverlap();
}

bool GeneratedMap::validateInRegion() const
{
    int i = 0;
    for (auto& tile: m_tiles)
    {
        if (tile.outsideRegion(0.0f, 0.0f, m_mapWidth, m_mapHeight))
        {
            LOG_ERROR("Tile %d outside map: %s (%f %f %f %f %f)", i, tile.m_name->c_str(),
                      tile.m_left, tile.m_bottom, tile.m_right, tile.m_top);
            return false;
        }
        ++i;
    }

    i = 0;
    for (auto& robot: m_robots)
    {
        if (robot.outsideRegion(0.0f, 0.0f, m_mapWidth, m_mapHeight))
        {
            LOG_ERROR("Robot %d outside map: %s (%f %f %f %f)", i, robot.m_name->c_str(),
                      robot.m_left, robot.m_bottom, robot.m_right, robot.m_top);
            return false;
        }
        ++i;
    }

    return true;
}

bool GeneratedMap::validateOverlap() const
{
    int i = 0;
    for (auto it = m_tiles.begin(); it != m_tiles.end(); ++it, ++i)
    {
        int j = i + 1;
        auto it1 = it;
        for(++it1; it1 != m_tiles.end(); ++it1, ++j)
        {
            if (it->overlap(*it1))
            {
                LOG_ERROR("Tile %d (%s %f %f %f %f) overlaps with tile %d (%s %f %f %f %f)",
                          i, it->m_name->c_str(), it->m_left, it->m_bottom, it->m_right, it->m_top,
                          j, it1->m_name->c_str(), it1->m_left, it1->m_bottom, it1->m_right, it1->m_top);
                return false;
            }
        }
    }

    i = 0;
    for (auto it = m_robots.begin(); it != m_robots.end(); ++it, ++i)
    {
        int j = i + 1;
        auto it1 = it;
        for(++it1; it1 != m_robots.end(); ++it1, ++j)
        {
            if (it->overlap(*it1))
            {
                LOG_ERROR("Robot %d (%s %f %f %f %f) overlaps with robot %d (%s %f %f %f %f)",
                          i, it->m_name->c_str(), it->m_left, it->m_bottom, it->m_right, it->m_top,
                          j, it1->m_name->c_str(), it1->m_left, it1->m_bottom, it1->m_right, it1->m_top);
                return false;
            }
        }
    }

    i = 0;
    for (auto it = m_tiles.begin(); it != m_tiles.end(); ++it, ++i)
    {
        int j = 0;
        for (auto it1 = m_robots.begin(); it1 != m_robots.end(); ++it1, ++j)
        {
            if (it->overlap(*it1))
            {
                LOG_ERROR("Tile %d (%s %f %f %f %f) overlaps with robot %d (%s %f %f %f %f)",
                          i, it->m_name->c_str(), it->m_left, it->m_bottom, it->m_right, it->m_top,
                          j, it1->m_name->c_str(), it1->m_left, it1->m_bottom, it1->m_right, it1->m_top);
                return false;
            }
        }
    }

    return true;
}

} // end of namespace bot

