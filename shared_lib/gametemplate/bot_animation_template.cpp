#include "gametemplate/bot_animation_template.h"

namespace bot {

AnimationTemplate::AnimationTemplate()
    : GameObjectTemplate(GAME_OBJ_TYPE_ANIMATION)
    , m_frameInterval(0.0f)
    , m_rect(nullptr)
{
}

void AnimationTemplate::setNumFrames(int numFrames)
{
    m_frames.resize(numFrames);
}

} // end of namespace bot
