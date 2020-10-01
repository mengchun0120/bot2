#include <algorithm>
#include "gameobj/bot_fire_point.h"

namespace bot {

FirePoint::FirePoint()
{
    std::fill(m_firePos, m_firePos + sizeof(m_firePos), 0.0f);
    std::fill(m_fireDirection, m_fireDirection + sizeof(m_fireDirection), 0.0f);
}

FirePoint::FirePoint(const FirePoint& fp)
{
    *this = fp;
}

FirePoint& FirePoint::operator=(const FirePoint& fp)
{
    std::copy(fp.m_firePos, fp.m_firePos + sizeof(fp.m_firePos), m_firePos);
    std::copy(fp.m_fireDirection, fp.m_fireDirection + sizeof(fp.m_fireDirection),
              m_fireDirection);
    return *this;
}



} // end of namespace bot

