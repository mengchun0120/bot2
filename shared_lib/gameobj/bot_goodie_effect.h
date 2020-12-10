#ifndef INCLUDE_BOT_GOODIE_EFFECT
#define INCLUDE_BOT_GOODIE_EFFECT

#include "misc/bot_time_utils.h"
#include "misc/bot_constants.h"
#include "gametemplate/bot_goodie_template.h"

namespace bot {

class GoodieEffect {
public:
    GoodieEffect()
        : m_template(nullptr)
        , m_percent(0.0f)
        , m_next(nullptr)
    {}

    ~GoodieEffect()
    {}

    const GoodieTemplate* getTemplate() const
    {
        return m_template;
    }

    void setTemplate(const GoodieTemplate* t)
    {
        m_template = t;
    }

    GoodieType getType() const
    {
        return m_template->getGoodieType();
    }

    GoodieEffect* getNext() const
    {
        return m_next;
    }

    void setNext(GoodieEffect* next)
    {
        m_next = next;
    }

    float getWidth() const
    {
        return m_template->getWidth();
    }

    float getHeight() const
    {
        return m_template->getHeight();
    }

    float getRadius() const
    {
        return m_template->getProgressRing()->getRadius();
    }

    void start();

    void setPos(float x, float y);

    void draw() const;

    const TimePoint& getStartTime() const
    {
        return m_startTime;
    }

    bool update(const TimePoint& now);

private:
    float m_pos[Constants::NUM_FLOATS_PER_POSITION];
    const GoodieTemplate* m_template;
    TimePoint m_startTime;
    float m_percent;
    GoodieEffect* m_next;
};

} // end of namespace bot

#endif
