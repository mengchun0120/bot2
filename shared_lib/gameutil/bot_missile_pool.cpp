#include "misc/bot_log.h"
#include "gameutil/bot_missile_pool.h"

namespace bot {

bool MissilePool::init(const std::vector<int> poolSizes)
{
    if (poolSizes.size() != MISSILE_COUNT)
    {
        LOG_ERROR("poolSizes is invalid");
        return false;
    }

    for (int i = 0; i < MISSILE_COUNT; ++i)
    {
        if (poolSizes[i] <= 0)
        {
            LOG_ERROR("poolSizes[%d]=%d is invalid", i, poolSizes[i]);
            return false;
        }
    }

    m_bulletPool.init(poolSizes[MISSILE_BULLET]);
    m_shellPool.init(poolSizes[MISSILE_SHELL]);
    m_deckPiercerPool.init(poolSizes[MISSILE_DECK_PIERCER]);

    return true;
}

Missile* MissilePool::alloc(const MissileTemplate* t, Side side,
                            float x, float y,
                            float directionX, float directionY,
                            float damage, float speed)
{
    Missile* missile = nullptr;

    switch (t->getMissileType())
    {
        case MISSILE_BULLET:
        {
            Bullet* bullet = m_bulletPool.alloc();
            const BulletTemplate* t1 = static_cast<const BulletTemplate*>(t);

            bool ret = bullet->init(t1, side, x, y, directionX, directionY,
                                    damage, speed);
            if (!ret)
            {
                LOG_ERROR("Failed to initialize bullet");
                m_bulletPool.free(bullet);
                return nullptr;
            }

            missile = bullet;

            break;
        }
        case MISSILE_SHELL:
        {
            Shell* shell = m_shellPool.alloc();
            const ShellTemplate* t1 = static_cast<const ShellTemplate*>(t);

            bool ret = shell->init(t1, side, x, y, directionX, directionY,
                                   damage, speed);
            if (!ret)
            {
                LOG_ERROR("Failed to initialize shell");
                m_shellPool.free(shell);
                return nullptr;
            }

            missile = shell;

            break;
        }
        case MISSILE_DECK_PIERCER:
        {
            DeckPiercer* deckPiercer = m_deckPiercerPool.alloc();
            const DeckPiercerTemplate* t1 =
                                static_cast<const DeckPiercerTemplate*>(t);

            bool ret = deckPiercer->init(t1, side, x, y, directionX, directionY,
                                         damage, speed);
            if (!ret)
            {
                LOG_ERROR("Failed to initialize deck-piercer");
                m_deckPiercer.free(deckPiercer);
                return nullptr;
            }

            missile = deckPiercer;

            break;
        }
        default:
        {
            LOG_ERROR("Invalid missile type %d",
                      static_cast<int>(t->getMissileType()));
        }
    }

    return missile;
}

void MissilePool::free(Missile* missile)
{
    switch (missile->getMissileType())
    {
        case MISSILE_BULLET:
        {
            Bullet* bullet = static_cast<Bullet*>(missile);
            m_bulletPool.free(bullet);
            break;
        }
        case MISSILE_SHELL:
        {
            Shell* shell = static_cast<Shell*>(missile);
            m_shellPool.free(shell);
            break;
        }
        case MISSILE_DECK_PIERCER:
        {
            DeckPiercer* deckPiercer = static_cast<DeckPiercer*>(missile);
            m_deckPiercer.free(deckPiercer);
            break;
        }
        default:
        {
            LOG_ERROR("Invalid missile-type: %d",
                      static_cast<int>(missile->getMissileType()));
        }
    }
}

} // end of namespace bot

