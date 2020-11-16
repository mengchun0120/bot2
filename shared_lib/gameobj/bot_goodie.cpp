#include "opengl/bot_texture.h"
#include "geometry/bot_rectangle.h"
#include "gameobj/bot_goodie.h"

namespace bot {

bool Goodie::init(const GoodieTemplate* t, float x, float y)
{
    return GameObject::init(t, x, y);
}

void Goodie::present()
{
    const GoodieTemplate* t = getTemplate();

    t->getRect()->draw(m_pos, nullptr, nullptr, nullptr,
                       t->getTexture()->textureId(), nullptr);
}

} // end of namespace bot
