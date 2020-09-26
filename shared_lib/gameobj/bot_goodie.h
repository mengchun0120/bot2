#ifndef INCLUDE_BOT_GOODIE
#define INCLUDE_BOT_GOODIE

#include "gameobj/bot_game_object.h"
#include "gametemplate/bot_goodie_template.h"

namespace bot {

class Goodie : public GameObject {
public:
    Goodie()
    {}

    virtual ~Goodie()
    {}

    bool init(const GoodieTemplate* t, float x, float y);

    const GoodieTemplate* getTemplate() const
    {
        return static_cast<const GoodieTemplate*>(m_template);
    }

    GoodieType getGoodieType() const
    {
        return getTemplate()->getGoodieType();
    }

    virtual void present(Graphics& g);

    virtual void update(float delta, GameScreen& screen)
    {}
};

} // end of namespace bot

#endif
