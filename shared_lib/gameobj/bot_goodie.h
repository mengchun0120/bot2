#ifndef INCLUDE_BOT_GOODIE
#define INCLUDE_BOT_GOODIE

#include "misc/bot_time_utils.h"
#include "gameobj/bot_game_object.h"
#include "gametemplate/bot_goodie_template.h"

namespace bot {

class Goodie : public GameObject {
public:
    Goodie(const GoodieTemplate* t, float x, float y);

    virtual ~Goodie();

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

private:
    TimePoint m_startTime;
};

} // end of namespace bot

#endif