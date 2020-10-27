#ifndef INCLUDE_BOT_GAME_MAP
#define INCLUDE_BOT_GAME_MAP

#include <vector>
#include "misc/bot_return_code.h"
#include "misc/bot_constants.h"
#include "structure/bot_object_pool.h"
#include "structure/bot_linked_list.h"
#include "gameutil/bot_game_object_item.h"
#include "gameobj/bot_game_object_flag.h"

namespace bot {

class Player;
class Robot;
class Missile;

class GameMap {
public:
    static int getMapCoord(float z)
    {
        return static_cast<int>(z / GRID_BREATH);
    }

    GameMap();

    virtual ~GameMap();

    void initMap(int numRows, int numCols, int gameObjPoolSize,
                 float viewportWidth, float viewportHeight);

    void clear();

    void present();

    int getNumRows() const
    {
        return static_cast<int>(m_map.size());
    }

    int getNumCols() const
    {
        return static_cast<int>(m_map[0].size());
    }

    float getMapWidth() const
    {
        return m_mapWidth;
    }

    float getMapHeight() const
    {
        return m_mapHeight;
    }

    LinkedList<GameObjectItem>& getMapCell(int row, int col)
    {
        return m_map[row][col];
    }

    const LinkedList<GameObjectItem>& getMapCell(int row, int col) const
    {
        return m_map[row][col];
    }

    bool getMapPosForGameObj(int& startRow, int& endRow,
                             int& startCol, int& endCol,
                             GameObject* obj) const;

    bool getRectCoords(int& startRow, int& endRow,
                       int& startCol, int& endCol,
                       float left, float bottom,
                       float right, float top) const;

    bool addObject(GameObject* obj);

    void removeObject(GameObject* obj);

    bool repositionObject(GameObject* obj);

    void getViewportRegion(int& startRow, int& endRow,
                           int& startCol, int& endCol) const;

    void clearFlagsInRect(int startRow, int endRow,
                          int startCol, int endCol,
                          GameObjectFlag flag);

    void setPlayer(Player* player);

    Player* getPlayer()
    {
        return m_player;
    }

    const Player* getPlayer() const
    {
        return m_player;
    }

    bool checkCollision(float& newDelta,
                        LinkedList<GameObjectItem>* collideObjs,
                        const Robot* robot,
                        float speedX, float speedY,
                        float delta);

    // Returns RET_CODE_OUT_OF_SIGHT, RET_CODE_COLLIDE and RETCODE_OK
    ReturnCode checkCollision(const Missile* missile);

    void freeGameObjList(LinkedList<GameObjectItem>& objs);

    bool isOutsideScreen(const GameObject* obj) const;

    bool isOutsideViewport(const GameObject* obj) const;

    void updateViewport();

    void setViewportPos(float x, float y);

    const float* getViewportPos() const
    {
        return m_viewportPos;
    }

    float getViewportLeft() const
    {
        return m_viewportPos[0] - m_viewportBreathX;
    }

    float getViewportRight() const
    {
        return m_viewportPos[0] + m_viewportBreathX;
    }

    float getViewportBottom() const
    {
        return m_viewportPos[1] - m_viewportBreathY;
    }

    float getViewportTop() const
    {
        return m_viewportPos[1] + m_viewportBreathY;
    }

    float getWorldX(float x) const
    {
        return x + m_viewportWorldX;
    }

    float getWorldY(float y) const
    {
        return y + m_viewportWorldY;
    }

public:
    static const float GRID_BREATH;

protected:
    void addObjectToRect(GameObject* obj, int startRow, int endRow,
                         int startCol, int endCol);

    bool removeObjectAt(GameObject* obj, int row, int col);

    void removeObjectFromRect(GameObject* obj, int startRow, int endRow,
                              int startCol, int endCol);

    bool checkCollideNonPassthrough(float& newDelta, const Robot* robot,
                                    float speedX, float speedY, float delta);

    void checkCollidePassthrough(LinkedList<GameObjectItem>* collideObjs,
                                 const Robot* robot, float speedX, float speedY,
                                 float delta);

    void getCollideRegion(int& startRow, int& endRow, int& startCol,
                          int& endCol, const GameObject* obj,
                          float speedX, float speedY, float delta);

protected:
    ObjectPool<GameObjectItem> m_gameObjItemPool;
    std::vector<std::vector<LinkedList<GameObjectItem>>> m_map;
    float m_mapWidth, m_mapHeight;
    Player* m_player;
    float m_viewportBreathX, m_viewportBreathY;
    float m_maxViewportX, m_maxViewportY;
    float m_viewportWorldX, m_viewportWorldY;
    float m_viewportPos[Constants::NUM_FLOATS_PER_POSITION];
};

} // end of namespace bot

#endif
