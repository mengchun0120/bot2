#include <algorithm>
#include "gameobj/bot_fire_point.h"

namespace bot {

FirePoint::FirePoint()
{
    for(unsigned int i = 0; i < Constants::NUM_FLOATS_PER_POSITION; ++i)
    {
        m_firePos[i] = 0.0f;
        m_fireDirection[i] = 0.0f;
    }
}

FirePoint::FirePoint(const FirePoint &fp)
{
    *this = fp;
}

FirePoint &FirePoint::operator=(const FirePoint &fp)
{
    for(unsigned int i = 0; i < Constants::NUM_FLOATS_PER_POSITION; ++i)
    {
        m_firePos[i] = fp.m_firePos[i];
        m_fireDirection[i] = fp.m_fireDirection[i];
    }
    return *this;
}



} // end of namespace bot

