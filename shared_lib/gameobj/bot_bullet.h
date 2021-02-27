#ifndef INCLUDE_BOT_BULLET
#define INCLUDE_BOT_BULLET

#include "gameobj/bot_missile.h"
#include "gameobj/bot_bullet_template.h"

namespace bot {

template <typename T> class LinkedList;
class GameObjectItem;

class Bullet: public Missile {
public:
    Bullet();

    virtual ~Bullet();

    bool init(const BulletTemplate *t, Side side, float x, float y,
              float directionX, float directionY, float damage, float speed);

    const BulletTemplate *getTemplate() const
    {
        return static_cast<const BulletTemplate*>(m_template);
    }

    virtual void present();

    virtual void update(float delta, GameScreen &screen);

    virtual bool onEntry(GameScreen &screen);

    virtual void onHit(GameScreen &screen, GameObject &obj);

    virtual void onDeath(GameScreen &screen);

private:
    bool checkCollision(GameScreen &screen);

    void processCollideObjs(GameScreen &screen,
                            LinkedList<GameObjectItem> &collideObjs);
};

} // end of namespace bot

#endif

