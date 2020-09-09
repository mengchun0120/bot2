#include "gameobj/bot_base_component.h"

namespace bot {

BaseComponent::BaseComponent(const BaseComponentTemplate* t)
    : Component(t)
{
}

bool BaseComponent::update()
{
    return true;
}

void BaseComponent::addHP(float delta)
{
}

void BaseComponent::addArmor(float delta)
{
}

void BaseComponent::addPower(float delta)
{
}

} // end of namespace bot

