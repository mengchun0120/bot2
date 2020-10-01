#ifndef INCLUDE_BOT_FIRE_POINT
#define INCLUDE_BOT_FIRE_POINT

#include "misc/bot_constants.h"

namespace bot {

struct FirePoint {
    FirePoint();

    FirePoint(const FirePoint& fp);

    ~FirePoint()
    {}

    FirePoint& operator=(const FirePoint& fp);

    float m_firePos[Constants::NUM_FLOATS_PER_POSITION];
    float m_fireDirection[Constants::NUM_FLOATS_PER_POSITION];
};

} // end of namespace bot

#endif

