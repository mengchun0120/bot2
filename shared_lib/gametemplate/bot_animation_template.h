#ifndef INCLUDE_BOT_ANIMATION_TEMPLATE
#define INCLUDE_BOT_ANIMATION_TEMPLATE

#include <vector>
#include "opengl/bot_texture.h"
#include "gametemplate/bot_game_object_template.h"

namespace bot {

class Rectangle;

class AnimationTemplate : public GameObjectTemplate {
public:
    AnimationTemplate();

    virtual ~AnimationTemplate()
    {}

    int getNumFrames() const
    {
        return static_cast<int>(m_frames.size());
    }

    void setNumFrames(int numFrames);

    const Texture& getFrame(int idx) const
    {
        return m_frames[idx];
    }

    Texture& getFrame(int idx)
    {
        return m_frames[idx];
    }

    float getFrameInterval() const
    {
        return m_frameInterval;
    }

    void setFrameInterval(float frameInterval)
    {
        m_frameInterval = frameInterval;
    }

    const Rectangle* getRect() const
    {
        return m_rect;
    }

    void setRect(const Rectangle* rect)
    {
        m_rect = rect;
    }

protected:
    std::vector<Texture> m_frames;
    float m_frameInterval;
    const Rectangle* m_rect;
};

} // end of namespace bot

#endif
