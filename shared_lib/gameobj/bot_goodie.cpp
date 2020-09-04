#include "misc/bot_log.h"
#include "geometry/bot_rectangle.h"
#include "opengl/bot_texture.h"
#include "gameobj/bot_goodie.h"

namespace bot {

Goodie::Goodie(const GoodieTemplate* t, float x, float y)
    : GameObject(t)
    , m_startTime(Clock::now())
{
    m_pos[0] = x;
    m_pos[1] = y;
}

Goodie::~Goodie()
{
}

void Goodie::present(Graphics& g)
{
    const GoodieTemplate* t = getTemplate();
    t->getRect()->draw(g, m_pos, nullptr, nullptr, nullptr, t->getTexture()->textureId(), nullptr);
}

} // end of namespace bot
