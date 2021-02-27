#ifndef INCLUDE_BOT_MISSILE_POOL
#define INCLUDE_BOT_MISSILE_POOL

#include "structure/bot_object_pool.h"
#include "gameobj/bot_bullet.h"
#include "gameobj/bot_shell.h"
#include "gameobj/bot_deck_piercer.h"

namespace bot {

class MissilePool {
public:
    MissilePool()
    {}

    ~MissilePool()
    {}

    bool init(int poolSize);

    Missile *alloc(const MissileTemplate *t, Side side, float x, float y,
                   float directionX, float directionY,
                   float damage, float speed);

    void free(Missile *missile);

private:
    ObjectPool<Bullet> m_bulletPool;
    ObjectPool<Shell> m_shellPool;
    ObjectPool<DeckPiercer> m_deckPiercerPool;
};

} // end of namespace bot

#endif

