#ifndef INCLUDE_DECK_PIERCER
#define INCLUDE_DECK_PIERCER

#include "structure/bot_linked_list.h"
#include "gameobj/bot_deck_piercer_template.h"
#include "gameobj/bot_missile.h"
#include "gameutil/bot_game_object_item.h"

namespace bot {

class DeckPiercer: public Missile {
public:
    DeckPiercer();

    virtual ~DeckPiercer();

    bool init(const DeckPiercerTemplate *t, Side side, float x, float y,
              float directionX, float directionY, float damage, float speed);

    const DeckPiercerTemplate *getTemplate() const
    {
        return static_cast<const DeckPiercerTemplate*>(m_template);
    }

    virtual void present();

    virtual void update(float delta, GameScreen &screen);

    virtual bool onEntry(GameScreen &screen);

    virtual void onHit(GameScreen &screen, GameObject &obj);

    virtual void onDeath(GameScreen &screen);

    virtual void onDealloc();

private:
    bool checkCollision(GameScreen &screen);

    bool processPenetrateObjs(LinkedList<GameObjectItem> &collideObjs,
                              GameScreen &screen);

private:
    LinkedList<GameObjectItem> m_penetrateObjs;
};

} // end of namespace bot

#endif

