#include "opengl/bot_texture.h"
#include "geometry/bot_rectangle.h"
#include "opengl/bot_graphics.h"
#include "gametemplate/bot_component_template.h"
#include "gameobj/bot_component.h"

namespace bot {

void Component::present(Graphics& g, const float* pos, const float* direction)
{
    m_template->getRect()->draw(g, pos, direction, nullptr, nullptr,
                                m_template->getTexture()->textureId(), nullptr);

}

} // end of namespace bot

