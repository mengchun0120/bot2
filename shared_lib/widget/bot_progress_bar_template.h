#ifndef INCLUDE_BOT_PROGRESS_BAR_TEMPLATE
#define INCLUDE_BOT_PROGRESS_BAR_TEMPLATE

#include <string>
#include <rapidjson/document.h>
#include "misc/bot_constants.h"
#include "opengl/bot_color.h"
#include "geometry/bot_rectangle.h"

namespace bot {

class Texture;

class ProgressBarTemplate {
public:
    ProgressBarTemplate();

    ~ProgressBarTemplate()
    {}

    bool init(const rapidjson::Value &elem);

    const Texture *getTexture() const
    {
        return m_texture;
    }

    const Color &getColor() const
    {
        return m_color;
    }

    const Rectangle *getRect() const
    {
        return m_rect;
    }

    float getWidth() const
    {
        return m_rect->width();
    }

    float getHeight() const
    {
        return m_rect->height();
    }

    float getStartOffsetX() const
    {
        return m_startOffset[0];
    }

    float getStartOffsetY() const
    {
        return m_startOffset[1];
    }

    float getBarWidth() const
    {
        return m_barWidth;
    }

    float getBarHeight() const
    {
        return m_barHeight;
    }

    float getSlotCount() const
    {
        return m_slotCount;
    }

private:
    const Texture *m_texture;
    Color m_color;
    const Rectangle *m_rect;
    float m_startOffset[Constants::NUM_FLOATS_PER_POSITION];
    float m_barWidth;
    float m_barHeight;
    int m_slotCount;
};

} // end of namespace bot

#endif

