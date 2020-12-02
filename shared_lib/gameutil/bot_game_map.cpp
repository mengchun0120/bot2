#include <algorithm>
#include "misc/bot_log.h"
#include "misc/bot_math_utils.h"
#include "gameobj/bot_player.h"
#include "gameobj/bot_missile.h"
#include "gameutil/bot_collide.h"
#include "gameutil/bot_game_map.h"

namespace bot {

const float GameMap::GRID_BREATH = 40.0f;

GameMap::GameMap()
    : m_mapWidth(0.0f)
    , m_mapHeight(0.0f)
    , m_player(nullptr)
    , m_viewportBreathX(0.0f)
    , m_viewportBreathY(0.0f)
    , m_maxViewportX(0.0f)
    , m_maxViewportY(0.0f)
    , m_viewportWorldX(0.0f)
    , m_viewportWorldY(0.0f)
    , m_aiRobotCount(0)
{
    m_viewportPos[0] = 0.0f;
    m_viewportPos[1] = 0.0f;
}

GameMap::~GameMap()
{
    clear();
}

void GameMap::initMap(int numRows, int numCols, int gameObjPoolSize,
                      float viewportWidth, float viewportHeight)
{
    m_gameObjItemPool.init(gameObjPoolSize);

    m_map.resize(numRows);
    for (int r = 0; r < numRows; ++r)
    {
        m_map[r].resize(numCols);
    }

    m_mapWidth = numCols * GRID_BREATH;
    m_mapHeight = numRows * GRID_BREATH;

    m_viewportBreathX = viewportWidth / 2.0f;
    m_viewportBreathY = viewportHeight / 2.0f;

    m_maxViewportX = m_mapWidth - m_viewportBreathX;
    m_maxViewportY = m_mapHeight - m_viewportBreathY;
}

void GameMap::present()
{
    static const GameObjectType LAYER_ORDER[] = {
        GAME_OBJ_TYPE_TILE,
        GAME_OBJ_TYPE_GOODIE,
        GAME_OBJ_TYPE_MISSILE,
        GAME_OBJ_TYPE_ROBOT
    };
    static const int NUM_LAYERS = sizeof(LAYER_ORDER) / sizeof(GameObjectType);
    static const int DONT_DRAW_FLAGS = GAME_OBJ_FLAG_DRAWN | GAME_OBJ_FLAG_DEAD;

    int startRow, endRow, startCol, endCol;

    getViewportRegion(startRow, endRow, startCol, endCol);
    clearFlagsInRect(startRow, endRow, startCol, endCol, GAME_OBJ_FLAG_DRAWN);

    for (int i = 0; i < NUM_LAYERS; ++i)
    {
        for (int r = startRow; r <= endRow; ++r)
        {
            for (int c = startCol; c <= endCol; ++c)
            {
                LinkedList<GameObjectItem>& cell = m_map[r][c];
                GameObjectItem* item, * next;
                for (item = cell.getFirst(); item; item = next)
                {
                    next = static_cast<GameObjectItem*>(item->getNext());

                    GameObject* obj = item->getObj();
                    bool dontShow = obj->getType() != LAYER_ORDER[i] ||
                                    obj->testFlag(DONT_DRAW_FLAGS);
                    if (dontShow)
                    {
                        continue;
                    }

                    obj->present();
                    obj->setFlag(GAME_OBJ_FLAG_DRAWN);
                }
            }
        }
    }
}

void GameMap::clear()
{
    int numRows = getNumRows();
    int numCols = getNumCols();

    for (int r = 0; r < numRows; ++r)
    {
        std::vector<LinkedList<GameObjectItem>>& row = m_map[r];
        for (int c = 0; c < numCols; ++c)
        {
            freeGameObjList(row[c]);
        }
    }
}

bool GameMap::getMapPosForGameObj(int& startRow, int& endRow, int& startCol,
                                  int& endCol, GameObject* obj) const
{
    return getRectCoords(startRow, endRow, startCol, endCol,
                         obj->getCoverLeft(), obj->getCoverBottom(),
                         obj->getCoverRight(), obj->getCoverTop());
}

bool GameMap::getRectCoords(int& startRow, int& endRow,
                            int& startCol, int& endCol,
                            float left, float bottom,
                            float right, float top) const
{
    int endCol1 = getMapCoord(right);
    if (endCol1 < 0)
    {
        return false;
    }

    int numCols = getNumCols();
    int startCol1 = getMapCoord(left);
    if (startCol1 >= numCols)
    {
        return false;
    }

    int endRow1 = getMapCoord(top);
    if (endRow1 < 0)
    {
        return false;
    }

    int numRows = getNumRows();
    int startRow1 = getMapCoord(bottom);
    if (startRow1 >= numRows)
    {
        return false;
    }

    if (startCol1 < 0) {
        startCol1 = 0;
    }

    if (endCol1 >= numCols) {
        endCol1 = numCols - 1;
    }

    if (startRow1 < 0) {
        startRow1 = 0;
    }

    if (endRow1 >= numRows) {
        endRow1 = numRows - 1;
    }

    startRow = startRow1;
    endRow = endRow1;
    startCol = startCol1;
    endCol = endCol1;

    return true;
}

bool GameMap::addObject(GameObject* obj)
{
    int startRow, endRow, startCol, endCol;

    bool placeObjInMap = getMapPosForGameObj(startRow, endRow,
                                             startCol, endCol, obj);
    if (!placeObjInMap)
    {
        return false;
    }

    addObjectToRect(obj, startRow, endRow, startCol, endCol);
    obj->setCoverRect(startRow, endRow, startCol, endCol);

    if (obj->getType() == GAME_OBJ_TYPE_ROBOT)
    {
        Robot* robot = static_cast<Robot*>(obj);
        if (robot->getSide() == SIDE_AI)
        {
            ++m_aiRobotCount;
        }
    }

    return true;
}

void GameMap::removeObject(GameObject* obj)
{
    removeObjectFromRect(obj, obj->getCoverStartRow(), obj->getCoverEndRow(),
                         obj->getCoverStartCol(), obj->getCoverEndCol());

    if (obj->getType() == GAME_OBJ_TYPE_ROBOT)
    {
        Robot* robot = static_cast<Robot*>(obj);
        if (robot->getSide() == SIDE_AI)
        {
            --m_aiRobotCount;
        }
    }
}

bool GameMap::repositionObject(GameObject* obj)
{
    int newStartRow, newEndRow, newStartCol, newEndCol;

    bool placeObjInMap = getMapPosForGameObj(newStartRow, newEndRow,
                                             newStartCol, newEndCol, obj);
    if (!placeObjInMap)
    {
        removeObject(obj);
        return false;
    }

    int oldStartRow = obj->getCoverStartRow();
    int oldEndRow = obj->getCoverEndRow();
    int oldStartCol = obj->getCoverStartCol();
    int oldEndCol = obj->getCoverEndCol();

    bool noOverlap = newStartRow > oldEndRow ||
                      newEndRow < oldStartRow ||
                      newStartCol > oldEndCol ||
                      newEndCol < oldStartCol;

    if (noOverlap)
    {
        removeObjectFromRect(obj, oldStartRow, oldEndRow,
                             oldStartCol, oldEndCol);
        addObjectToRect(obj, newStartRow, newEndRow, newStartCol, newEndCol);
        obj->setCoverRect(newStartRow, newEndRow, newStartCol, newEndCol);
        return true;
    }

    if (newStartRow < oldStartRow)
    {
        addObjectToRect(obj, newStartRow, oldStartRow - 1,
                        newStartCol, newEndCol);
    }
    else if (newStartRow > oldStartRow)
    {
        removeObjectFromRect(obj, oldStartRow, newStartRow - 1,
                             oldStartCol, oldEndCol);
    }

    if (newEndRow > oldEndRow)
    {
        addObjectToRect(obj, oldEndRow + 1, newEndRow, newStartCol, newEndCol);
    }
    else if (newEndRow < oldEndRow)
    {
        removeObjectFromRect(obj, newEndRow + 1, oldEndRow,
                             oldStartCol, oldEndCol);
    }

    int overlapStartRow = std::max(newStartRow, oldStartRow);
    int overlapEndRow = std::min(newEndRow, oldEndRow);

    if (newStartCol < oldStartCol)
    {
        addObjectToRect(obj, overlapStartRow, overlapEndRow, newStartCol,
                        oldStartCol - 1);
    }
    else if (newStartCol > oldStartCol)
    {
        removeObjectFromRect(obj, overlapStartRow, overlapEndRow, oldStartCol,
                             newStartCol - 1);
    }

    if (newEndCol > oldEndCol)
    {
        addObjectToRect(obj, overlapStartRow, overlapEndRow,
                        oldEndCol + 1, newEndCol);
    }
    else if (newEndCol < oldEndCol)
    {
        removeObjectFromRect(obj, overlapStartRow, overlapEndRow,
                             newEndCol + 1, oldEndCol);
    }

    obj->setCoverRect(newStartRow, newEndRow, newStartCol, newEndCol);

    return true;
}

void GameMap::addObjectToRect(GameObject* obj, int startRow, int endRow,
                              int startCol, int endCol)
{
    for (int r = startRow; r <= endRow; ++r)
    {
        std::vector<LinkedList<GameObjectItem>>& row = m_map[r];
        for (int c = startCol; c <= endCol; ++c)
        {
            GameObjectItem* item = m_gameObjItemPool.alloc();
            item->setObj(obj);
            row[c].add(item);
        }
    }
}

bool GameMap::removeObjectAt(GameObject* obj, int row, int col)
{
    GameObjectItem* prev = nullptr, * cur;
    LinkedList<GameObjectItem>& cell = m_map[row][col];

    cur = cell.getFirst();
    while (cur)
    {
        if (cur->getObj() == obj)
        {
            break;
        }
        prev = cur;
        cur = static_cast<GameObjectItem*>(cur->getNext());
    }

    if (!cur)
    {
        return false;
    }

    cell.unlink(prev, cur);
    m_gameObjItemPool.free(cur);

    return true;
}

void GameMap::removeObjectFromRect(GameObject* obj, int startRow, int endRow,
                                   int startCol, int endCol)
{
    for (int r = startRow; r <= endRow; ++r)
    {
        for (int c = startCol; c <= endCol; ++c)
        {
            removeObjectAt(obj, r, c);
        }
    }
}

void GameMap::getViewportRegion(int& startRow, int& endRow,
                                int& startCol, int& endCol) const
{
    float left = m_viewportPos[0] - m_viewportBreathX;
    float bottom = m_viewportPos[1] - m_viewportBreathY;
    float right = m_viewportPos[0] + m_viewportBreathX;
    float top = m_viewportPos[1] + m_viewportBreathY;
    getRectCoords(startRow, endRow, startCol, endCol, left, bottom, right, top);
}

void GameMap::clearFlagsInRect(int startRow, int endRow,
                               int startCol, int endCol,
                               GameObjectFlag flag)
{
    for (int r = startRow; r <= endRow; ++r)
    {
        std::vector<LinkedList<GameObjectItem>>& row = m_map[r];
        for (int c = startCol; c <= endCol; ++c)
        {
            GameObjectItem* item = row[c].getFirst();
            while (item)
            {
                item->getObj()->clearFlag(static_cast<int>(flag));
                item = static_cast<GameObjectItem*>(item->getNext());
            }
        }
    }
}

bool GameMap::setPlayer(Player* player)
{
    m_player = player;

    if (m_player)
    {
        if (!addObject(player))
        {
            LOG_ERROR("Failed to add player to map");
            return false;
        }

        updateViewport();
    }


    return true;
}

void GameMap::getCollideRegion(int& startRow, int& endRow,
                               int& startCol, int& endCol,
                               const GameObject* obj,
                               float speedX, float speedY,
                               float delta)
{
    float left = obj->getCollideLeft();
    float right = obj->getCollideRight();
    if (speedX < 0.0f)
    {
        left += speedX * delta;
    }
    else if (speedX > 0.0f)
    {
        right += speedX * delta;
    }

    float bottom = obj->getCollideBottom();
    float top = obj->getCollideTop();
    if (speedY < 0.0f)
    {
        bottom += speedY * delta;
    }
    else if (speedY > 0.0f)
    {
        top += speedY * delta;
    }

    startRow = getMapCoord(bottom);
    startRow = clamp(startRow, 0, getNumRows() - 1);
    endRow = getMapCoord(top);
    endRow = clamp(endRow, 0, getNumRows() - 1);
    startCol = getMapCoord(left);
    startCol = clamp(startCol, 0, getNumCols() - 1);
    endCol = getMapCoord(right);
    endCol = clamp(endCol, 0, getNumCols() - 1);
}

bool GameMap::checkCollision(float& newDelta,
                             LinkedList<GameObjectItem>* collideObjs,
                             const Robot* robot,
                             float speedX, float speedY,
                             float delta)
{
    bool touch = checkTouchBoundary(newDelta, m_mapWidth, m_mapHeight,
                                    robot->getPosX(), robot->getPosY(),
                                    robot->getCollideBreath(),
                                    robot->getCollideBreath(),
                                    speedX, speedY, delta);

    bool collide = checkCollideNonPassthrough(newDelta, robot, speedX, speedY,
                                              newDelta);
    if (collideObjs)
    {
        checkCollidePassthrough(collideObjs, robot, speedX, speedY, newDelta);
    }

    return touch || collide;
}

bool GameMap::checkCollideNonPassthrough(float& newDelta, const Robot* robot,
                                         float speedX, float speedY,
                                         float delta)
{
    const int DONT_CHECK_FLAG = GAME_OBJ_FLAG_CHECKED |
                                GAME_OBJ_FLAG_DEAD |
                                GAME_OBJ_FLAG_DISSOLVE;
    int startRow, endRow, startCol, endCol;
    bool collide = false;

    newDelta = delta;
    getCollideRegion(startRow, endRow, startCol, endCol, robot,
                     speedX, speedY, delta);
    clearFlagsInRect(startRow, endRow, startCol, endCol, GAME_OBJ_FLAG_CHECKED);

    for (int r = startRow; r <= endRow; ++r)
    {
        std::vector<LinkedList<GameObjectItem>>& row = m_map[r];
        for (int c = startCol; c <= endCol; ++c)
        {
            GameObjectItem* item = row[c].getFirst();
            while (item)
            {
                GameObject* o = item->getObj();
                bool dontCheck = o == static_cast<const GameObject*>(robot) ||
                                 o->testFlag(DONT_CHECK_FLAG) ||
                                 (o->getType() != GAME_OBJ_TYPE_ROBOT &&
                                  o->getType() != GAME_OBJ_TYPE_TILE);

                if (dontCheck)
                {
                    item = static_cast<GameObjectItem*>(item->getNext());
                    continue;
                }

                float newDelta1;
                bool collide1 = checkObjCollision(
                                newDelta1, robot->getPosX(),robot->getPosY(),
                                robot->getCollideBreath(),
                                robot->getCollideBreath(),
                                speedX, speedY, o->getPosX(), o->getPosY(),
                                o->getCollideBreath(), o->getCollideBreath(),
                                newDelta);

                if (collide1)
                {
                    collide = true;
                    if (newDelta1 < newDelta)
                    {
                        newDelta = newDelta1;
                    }
                }

                o->setFlag(GAME_OBJ_FLAG_CHECKED);
                item = static_cast<GameObjectItem*>(item->getNext());
            }
        }
    }

    return collide;
}

void GameMap::checkCollidePassthrough(LinkedList<GameObjectItem>* collideObjs,
                                      const Robot* robot,
                                      float speedX, float speedY,
                                      float delta)
{
    const int DONT_CHECK_FLAG = GAME_OBJ_FLAG_CHECKED | GAME_OBJ_FLAG_DEAD;
    int startRow, endRow, startCol, endCol;
    float deltaX = speedX * delta;
    float deltaY = speedY * delta;
    float left = robot->getCollideLeft() + deltaX;
    float right = robot->getCollideRight() + deltaX;
    float bottom = robot->getCollideBottom() + deltaY;
    float top = robot->getCollideTop() + deltaY;

    getCollideRegion(startRow, endRow, startCol, endCol, robot,
                     speedX, speedY, delta);
    clearFlagsInRect(startRow, endRow, startCol, endCol, GAME_OBJ_FLAG_CHECKED);

    for (int r = startRow; r <= endRow; ++r)
    {
        std::vector<LinkedList<GameObjectItem>>& row = m_map[r];
        for (int c = startCol; c <= endCol; ++c)
        {
            GameObjectItem* item = row[c].getFirst();
            while (item)
            {
                GameObject* o = item->getObj();
                bool check = (o->getType() == GAME_OBJ_TYPE_MISSILE ||
                              (robot->getSide() == SIDE_PLAYER &&
                               o->getType() == GAME_OBJ_TYPE_GOODIE)) &&
                               !o->testFlag(DONT_CHECK_FLAG);

                if (!check)
                {
                    item = static_cast<GameObjectItem*>(item->getNext());
                    continue;
                }

                bool collide = checkRectOverlapp(left, bottom, right, top,
                                                 o->getCollideLeft(),
                                                 o->getCollideBottom(),
                                                 o->getCollideRight(),
                                                 o->getCollideTop());

                if (collide)
                {
                    GameObjectItem* item = m_gameObjItemPool.alloc();
                    item->setObj(o);
                    collideObjs->add(item);
                }

                o->setFlag(GAME_OBJ_FLAG_CHECKED);
                item = static_cast<GameObjectItem*>(item->getNext());
            }
        }
    }
}

ReturnCode GameMap::checkCollision(const Missile* missile)
{
    if (isOutsideViewport(missile))
    {
        return RET_CODE_OUT_OF_SIGHT;
    }

    const int DONT_CHECK_FLAG = GAME_OBJ_FLAG_CHECKED |
                                GAME_OBJ_FLAG_DEAD |
                                GAME_OBJ_FLAG_DISSOLVE;
    int startRow, endRow, startCol, endCol;
    float left = missile->getCollideLeft();
    float bottom = missile->getCollideBottom();
    float right = missile->getCollideRight();
    float top = missile->getCollideTop();

    bool inBound = getRectCoords(startRow, endRow, startCol, endCol,
                                 left, bottom, right, top);
    if (!inBound)
    {
        return RET_CODE_OUT_OF_SIGHT;
    }

    clearFlagsInRect(startRow, endRow, startCol, endCol, GAME_OBJ_FLAG_CHECKED);

    for (int r = startRow; r <= endRow; ++r)
    {
        std::vector<LinkedList<GameObjectItem>>& row = m_map[r];
        for (int c = startCol; c <= endCol; ++c)
        {
            GameObjectItem* item = row[c].getFirst();
            while (item)
            {
                GameObject* o = item->getObj();
                bool dontCheck =
                  o == static_cast<const GameObject*>(missile) ||
                  o == static_cast<const GameObject*>(missile->getShooter()) ||
                  o->testFlag(DONT_CHECK_FLAG) ||
                  (o->getType() != GAME_OBJ_TYPE_ROBOT &&
                   o->getType() != GAME_OBJ_TYPE_TILE);

                if (dontCheck)
                {
                    item = static_cast<GameObjectItem*>(item->getNext());
                    continue;
                }

                bool collide = checkRectOverlapp(left, bottom, right, top,
                                                 o->getCollideLeft(),
                                                 o->getCollideBottom(),
                                                 o->getCollideRight(),
                                                 o->getCollideTop());

                if (collide)
                {
                    return RET_CODE_COLLIDE;
                }

                o->setFlag(GAME_OBJ_FLAG_CHECKED);
                item = static_cast<GameObjectItem*>(item->getNext());
            }
        }
    }

    return RET_CODE_OK;
}

void GameMap::freeGameObjList(LinkedList<GameObjectItem>& objs)
{
    GameObjectItem* next, * cur;
    for (cur = objs.getFirst(); cur; cur = next)
    {
        next = static_cast<GameObjectItem*>(cur->getNext());
        m_gameObjItemPool.free(cur);
    }
}

bool GameMap::isOutsideScreen(const GameObject* obj) const
{
    return obj->getCoverTop() <= 0.0f ||
           obj->getCoverBottom() >= m_mapHeight ||
           obj->getCoverRight() <= 0.0f ||
           obj->getCoverLeft() >= m_mapWidth;
}

bool GameMap::isOutsideViewport(const GameObject* obj) const
{
    return obj->getCoverTop() <= getViewportBottom() ||
           obj->getCoverBottom() >= getViewportTop() ||
           obj->getCoverRight() <= getViewportLeft() ||
           obj->getCoverLeft() >= getViewportRight();
}

void GameMap::updateViewport()
{
    if (!m_player)
    {
        return;
    }

    m_viewportPos[0] = clamp(m_player->getPosX(), m_viewportBreathX,
                             m_maxViewportX);
    m_viewportPos[1] = clamp(m_player->getPosY(), m_viewportBreathY,
                             m_maxViewportY);
    m_viewportWorldX = m_viewportPos[0] - m_viewportBreathX;
    m_viewportWorldY = m_viewportPos[1] - m_viewportBreathY;
}

void GameMap::setViewportPos(float x, float y)
{
    m_viewportPos[0] = clamp(x, m_viewportBreathX, m_maxViewportX);
    m_viewportPos[1] = clamp(y, m_viewportBreathY, m_maxViewportY);
    m_viewportWorldX = m_viewportPos[0] - m_viewportBreathX;
    m_viewportWorldY = m_viewportPos[1] - m_viewportBreathY;
}

} // end of namespace bot
