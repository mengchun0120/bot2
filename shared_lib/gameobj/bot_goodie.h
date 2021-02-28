#ifndef INCLUDE_BOT_GOODIE
#define INCLUDE_BOT_GOODIE

#include "gameobj/bot_game_object.h"
#include "gameobj/bot_goodie_template.h"

namespace bot {

class Goodie : public GameObject {
public:
    Goodie(const GoodieTemplate *t, float x, float y) noexcept(false)
        : GameObject(t, x, y)
    {}

    virtual ~Goodie()
    {}

    const GoodieTemplate *getTemplate() const
    {
        return static_cast<const GoodieTemplate*>(m_template);
    }

    GoodieType getGoodieType() const
    {
        return getTemplate()->getGoodieType();
    }

    virtual void present();

    virtual void update(float delta, GameScreen &screen)
    {}
};

} // end of namespace bot

#endif
