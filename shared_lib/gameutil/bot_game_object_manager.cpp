#include "misc/bot_log.h"
#include "gameobj/bot_player.h"
#include "gameutil/bot_game_map.h"
#include "gameutil/bot_game_lib.h"
#include "gameutil/bot_game_object_manager.h"
#include "app/bot_app_config.h"

namespace bot {

GameObjectManager::GameObjectManager()
    : m_map(nullptr)
    , m_lib(nullptr)
    , m_player(nullptr)
    , m_aiRobotCount(0)
{
}

GameObjectManager::~GameObjectManager()
{
    clearActiveObjects();
    clearDissolveObjects();
    clearDeadObjects();
    delete m_player;
}

void GameObjectManager::init(GameMap* map)
{
    const AppConfig& cfg = AppConfig::getInstance();
    m_map = map;
    m_lib = &(GameLib::getInstance());
    m_missilePool.init(cfg.getMissilePoolSize());
    m_goodieGenerator.init(m_lib->getGoodieTemplateLib());
}

Tile* GameObjectManager::createTile(const std::string& tileName, int level,
                                    float x, float y)
{
    const TileTemplate* tileTemplate = m_lib->getTileTemplate(tileName);
    if (!tileTemplate)
    {
        LOG_ERROR("Failed to find tile template %s", tileName.c_str());
        return nullptr;
    }

    return createTile(tileTemplate, level, x, y);
}

Tile* GameObjectManager::createTile(const TileTemplate* tileTemplate, int level,
                                    float x, float y)
{
    Tile* tile = new Tile();

    if (!tile->init(tileTemplate, level, x, y))
    {
        delete tile;
        return nullptr;
    }

    m_activeTiles.add(tile);

    return tile;
}

AIRobot* GameObjectManager::createRobot(
                         const std::string& robotName, Side side,
                         int hpLevel, int hpRestoreLevel,
                         int armorLevel, int armorRepairLevel,
                         int powerLevel, int powerRestoreLevel,
                         int weaponLevel, int missileLevel, int moverLevel,
                         float x, float y, float directionX, float directionY)
{
    const AIRobotTemplate* aiRobotTemplate =
                                 m_lib->getAIRobotTemplate(robotName);
    if (!aiRobotTemplate)
    {
        LOG_ERROR("Failed to find ai-robot template %s", robotName.c_str());
        return nullptr;
    }

    return createRobot(aiRobotTemplate, side,
                       hpLevel, hpRestoreLevel,
                       armorLevel, armorRepairLevel,
                       powerLevel, powerRestoreLevel,
                       weaponLevel, missileLevel, moverLevel,
                       x, y, directionX, directionY);
}

AIRobot* GameObjectManager::createRobot(
                         const AIRobotTemplate* aiRobotTemplate, Side side,
                         int hpLevel, int hpRestoreLevel,
                         int armorLevel, int armorRepairLevel,
                         int powerLevel, int powerRestoreLevel,
                         int weaponLevel, int missileLevel, int moverLevel,
                         float x, float y, float directionX, float directionY)
{
    AIRobot* robot = new AIRobot();
    bool ret = robot->init(aiRobotTemplate, side,
                           hpLevel, hpRestoreLevel,
                           armorLevel, armorRepairLevel,
                           powerLevel, powerRestoreLevel,
                           weaponLevel, missileLevel, moverLevel,
                           x, y, directionX, directionY);
    if (!ret)
    {
        delete robot;
        return nullptr;
    }

    m_activeRobots.add(robot);
    ++m_aiRobotCount;

    return robot;
}

Missile* GameObjectManager::createMissile(
                            const MissileTemplate* missileTemplate,
                            Robot* shooter, float damage,
                            float x, float y,
                            float directionX, float directionY)
{
    Missile* missile = m_missilePool.alloc();
    bool ret = missile->init(missileTemplate, shooter, damage,
                             x, y, directionX, directionY);
    if (!ret)
    {
        sendToDeathQueue(missile);
        return nullptr;
    }

    m_activeMissiles.add(missile);

    return missile;
}

ParticleEffect* GameObjectManager::createParticleEffect(
                            const ParticleEffectTemplate* t,
                            float x, float y)
{
    ParticleEffect* effect = m_particleEffectPool.alloc();
    bool ret = effect->init(t, x, y);
    if (!ret)
    {
        sendToDeathQueue(effect);
        return nullptr;
    }

    m_activeParticleEffect.add(effect);
    return effect;
}

Player* GameObjectManager::createPlayer(float x, float y,
                                        float directionX, float directionY)
{
    m_player = new Player();
    bool ret = m_player->init(&(m_lib->getPlayerTemplate()),
                              x, y,
                              directionX, directionY);
    if (!ret)
    {
        delete m_player;
        return nullptr;
    }

    return m_player;
}

Goodie* GameObjectManager::createGoodie(float prob, float x, float y)
{
    int goodieIdx = m_goodieGenerator.generate(prob);
    if (goodieIdx < 0)
    {
        return nullptr;
    }

    const GoodieTemplate* t = m_lib->getGoodieTemplateLib().getObjAt(goodieIdx);
    Goodie* goodie = new Goodie();
    bool ret = goodie->init(t, x, y);
    if (!ret)
    {
        delete goodie;
        return nullptr;
    }

    m_activeGoodies.add(goodie);
    return goodie;
}

bool GameObjectManager::isPlayerAlive() const
{
    const int DEAD_FLAG = GAME_OBJ_FLAG_DISSOLVE |
                          GAME_OBJ_FLAG_DEAD;
    return m_player && !m_player->testFlag(DEAD_FLAG);
}

void GameObjectManager::sendToDissolveQueue(GameObject* obj)
{
    switch (obj->getType())
    {
        case GAME_OBJ_TYPE_TILE:
        {
            Tile* tile = static_cast<Tile*>(obj);
            m_activeTiles.unlink(tile);
            m_dissolveObjects.add(obj);
            break;
        }
        case GAME_OBJ_TYPE_ROBOT:
        {
            Robot* robot = static_cast<Robot*>(obj);
            if (robot->getSide() == SIDE_AI)
            {
                m_activeRobots.unlink(robot);
                m_dissolveObjects.add(obj);
            }
            break;
        }
        default:
        {
            LOG_ERROR("Invalid object type %d for dissolve-queue:",
                      static_cast<int>(obj->getType()));
        }
    }

    obj->setFlag(GAME_OBJ_FLAG_DISSOLVE);
}

void GameObjectManager::sendToDeathQueue(GameObject* obj)
{
    switch (obj->getType())
    {
        case GAME_OBJ_TYPE_TILE:
        {
            Tile* tile = static_cast<Tile*>(obj);
            if (tile->testFlag(GAME_OBJ_FLAG_DISSOLVE))
            {
                m_dissolveObjects.unlink(tile);
                m_deadObjects.add(obj);
            }
            else
            {
                LOG_ERROR("Trying to send non-dissolving tile to death-queue");
            }
            break;
        }
        case GAME_OBJ_TYPE_ROBOT:
        {
            Robot* robot = static_cast<Robot*>(obj);
            if (robot->testFlag(GAME_OBJ_FLAG_DISSOLVE))
            {
                m_dissolveObjects.unlink(robot);
                m_deadObjects.add(robot);
                --m_aiRobotCount;
            }
            else
            {
                LOG_ERROR("Trying to send non-dissolving robot to death-queue");
            }
            break;
        }
        case GAME_OBJ_TYPE_MISSILE:
        {
            Missile* missile = static_cast<Missile*>(obj);
            m_activeMissiles.unlink(missile);
            m_deadObjects.add(obj);
            break;
        }
        case GAME_OBJ_TYPE_PARTICLE_EFFECT:
        {
            ParticleEffect* effect = static_cast<ParticleEffect*>(obj);
            m_activeParticleEffect.unlink(effect);
            m_deadObjects.add(obj);
            break;
        }
        case GAME_OBJ_TYPE_ANIMATION:
        {
            break;
        }
        case GAME_OBJ_TYPE_GOODIE:
        {
            Goodie* goodie = static_cast<Goodie*>(obj);
            m_activeGoodies.unlink(goodie);
            m_deadObjects.add(obj);
            break;
        }
        default:
        {
            LOG_ERROR("Invalid game obj type %d",
                      static_cast<int>(obj->getType()));
        }
    }

    obj->setFlag(GAME_OBJ_FLAG_DEAD);
}

void GameObjectManager::clearDeadObjects()
{
    auto deallocator = [this](GameObject* obj)
    {
        if (obj->getType() == GAME_OBJ_TYPE_MISSILE)
        {
            m_missilePool.free(static_cast<Missile*>(obj));
        }
        else if (obj->getType() == GAME_OBJ_TYPE_PARTICLE_EFFECT)
        {
            m_particleEffectPool.free(static_cast<ParticleEffect*>(obj));
        }
        else
        {
            delete obj;
        }
    };

    m_deadObjects.clear(deallocator);
}

void GameObjectManager::clearActiveObjects()
{
    m_activeTiles.clear();
    m_activeRobots.clear();
    m_activeGoodies.clear();

    auto missileDeallocator = [this](Missile* missile)
    {
        m_missilePool.free(missile);
    };

    m_activeMissiles.clear(missileDeallocator);

    auto particleEffectDeallocator = [this](ParticleEffect* effect)
    {
        m_particleEffectPool.free(effect);
    };

    m_activeParticleEffect.clear(particleEffectDeallocator);
}

void GameObjectManager::clearDissolveObjects()
{
    m_dissolveObjects.clear();
}

} // end of namespace bot

