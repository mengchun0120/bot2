#include "gameobj/bot_mover_component.h"

namespace bot {

MoverComponent::MoverComponent(const MoverComponentTemplate* t)
    : Component(t)
    , m_moving(false)
    , m_speedMultiplier(1.0f)
{
}

void MoverComponent::update(GameScreen& screen)
{
    if (!m_moving)
    {
        return;
    }
}

} // end of namespace bot

