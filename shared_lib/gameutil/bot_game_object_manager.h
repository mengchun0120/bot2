#ifndef INCLUDE_BOT_GAME_OBJECT_MANAGER
#define INCLUDE_BOT_GAME_OBJECT_MANAGER

#include <string>
#include "structure/bot_double_linked_list.h"
#include "structure/bot_object_pool.h"
#include "structure/bot_linked_list.h"
#include "gameobj/bot_tile.h"
#include "gameobj/bot_missile.h"
#include "gameobj/bot_particle_effect.h"
#include "gameobj/bot_goodie.h"
#include "gameobj/bot_side.h"
#include "gameobj/bot_ai_robot.h"
#include "gameutil/bot_game_object_item.h"
#include "gameutil/bot_goodie_generator.h"
#include "gameutil/bot_missile_pool.h"

namespace bot {

class GameLib;
class Player;
class AppConfig;
class GameLib;
class PlayerTemplate;
class Dashboard;

class GameObjectManager {
public:
    GameObjectManager();

    ~GameObjectManager();

    void init(Dashboard *dashboard);

    Tile *createTile(const std::string &tileName, int level,
                     float x, float y);

    Tile *createTile(const TileTemplate *tileTemplate, int level,
                     float x, float y);

    int getNumActiveAIRobots() const
    {
        return m_numActiveAIRobots;
    }

    Tile *getFirstActiveTile()
    {
        return m_activeTiles.getFirst();
    }

    const Tile *getFirstActiveTile() const
    {
        return m_activeTiles.getFirst();
    }

    GameObject *getFirstDissolveObject()
    {
        return m_dissolveObjects.getFirst();
    }

    const GameObject *getFirstDissolveObject() const
    {
        return m_dissolveObjects.getFirst();
    }

    AIRobot *createRobot(const std::string &robotName, Side side,
                         int hpLevel, int hpRestoreLevel,
                         int armorLevel, int armorRepairLevel,
                         int powerLevel, int powerRestoreLevel,
                         int weaponLevel, int moverLevel,
                         const std::vector<int> &skillLevels,
                         float x, float y, float directionX, float directionY);

    AIRobot *createRobot(const AIRobotTemplate *aiRobotTemplate, Side side,
                         int hpLevel, int hpRestoreLevel,
                         int armorLevel, int armorRepairLevel,
                         int powerLevel, int powerRestoreLevel,
                         int weaponLevel, int moverLevel,
                         const std::vector<int> &skillLevels,
                         float x, float y, float directionX, float directionY);

    AIRobot *createRobot(const std::string &robotName, Side side,
                         int hpLevel, int hpRestoreLevel,
                         int armorLevel, int armorRepairLevel,
                         int powerLevel, int powerRestoreLevel,
                         int weaponLevel, int moverLevel, int skillLevel,
                         float x, float y, float directionX, float directionY);

    AIRobot *createRobot(const AIRobotTemplate *aiRobotTemplate, Side side,
                         int hpLevel, int hpRestoreLevel,
                         int armorLevel, int armorRepairLevel,
                         int powerLevel, int powerRestoreLevel,
                         int weaponLevel, int moverLevel, int skillLevel,
                         float x, float y, float directionX, float directionY);

    Missile *createMissile(const MissileTemplate *missileTemplate,
                           Side side, float x, float y,
                           float directionX, float directionY,
                           float damage, float speed);

    Goodie *createGoodie(float prob, float x, float y);

    Missile *getFirstActiveMissile()
    {
        return m_activeMissiles.getFirst();
    }

    const Missile *getFirstActiveMissile() const
    {
        return m_activeMissiles.getFirst();
    }

    ParticleEffect *createParticleEffect(const ParticleEffectTemplate *t,
                                         float x, float y);

    ParticleEffect *getFirstParticleEffect()
    {
        return m_activeParticleEffect.getFirst();
    }

    const ParticleEffect *getFirstParticleEffect() const
    {
        return m_activeParticleEffect.getFirst();
    }

    Goodie *getFirstActiveGoodie()
    {
        return m_activeGoodies.getFirst();
    }

    const Goodie *getFirstActiveGoodie() const
    {
        return m_activeGoodies.getFirst();
    }

    GameObject *getFirstDeadObject()
    {
        return m_deadObjects.getFirst();
    }

    const GameObject *getFirstDeadObject() const
    {
        return m_deadObjects.getFirst();
    }

    Player *createPlayer(float x, float y,
                         float directionX, float directionY);

    const Player *getPlayer() const
    {
        return m_player;
    }

    Player *getPlayer()
    {
        return m_player;
    }

    bool isPlayerAlive() const;

    void sendToDissolveQueue(GameObject *obj);

    void sendToDeathQueue(GameObject *obj);

    void clearDeadObjects();

    void clearActiveObjects();

    void clearDissolveObjects();

    GameObjectItem *allocGameObjItem(GameObject *obj);

    void freeGameObjItem(GameObjectItem *item);

    void freeGameObjItems(LinkedList<GameObjectItem> &items);

protected:
    const GameLib *m_lib;
    GoodieGenerator m_goodieGenerator;
    MissilePool m_missilePool;
    ObjectPool<ParticleEffect> m_particleEffectPool;
    ObjectPool<GameObjectItem> m_gameObjItemPool;
    DoubleLinkedList<Robot> m_activeRobots;
    DoubleLinkedList<Tile> m_activeTiles;
    DoubleLinkedList<Missile> m_activeMissiles;
    DoubleLinkedList<ParticleEffect> m_activeParticleEffect;
    DoubleLinkedList<Goodie> m_activeGoodies;
    DoubleLinkedList<GameObject> m_dissolveObjects;
    DoubleLinkedList<GameObject> m_deadObjects;
    Player *m_player;
    int m_numActiveAIRobots;
    Dashboard *m_dashboard;
};

} // end of namespace bot

#endif
