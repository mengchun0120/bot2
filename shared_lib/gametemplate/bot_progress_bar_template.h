#ifndef INCLUDE_BOT_PROGRESS_BAR_TEMPLATE
#define INCLUDE_BOT_PROGRESS_BAR_TEMPLATE

#include <string>
#include <rapidjson/document.h>
#include "misc/bot_constants.h"

namespace bot {

template <typename T> class NamedMap;
class Texture;
class Color;
class Rectangle;

class ProgressBarTemplate {
public:
    class Parser {
    public:
        Parser(const NamedMap<Texture>& textureLib,
               const NamedMap<Color>& colorLib,
               const NamedMap<Rectangle>& rectLib)
            : m_textureLib(textureLib)
            , m_colorLib(colorLib)
            , m_rectLib(rectLib)
        {}

        ~Parser()
        {}

        ProgressBarTemplate* create(const std::string& name,
                                    const rapidjson::Value& elem);

    private:
        const NamedMap<Texture>& m_textureLib;
        const NamedMap<Color>& m_colorLib;
        const NamedMap<Rectangle>& m_rectLib;
    };

    ProgressBarTemplate()
        : m_texture(nullptr)
        , m_color(nullptr)
        , m_rect(nullptr)
        , m_barWidth(0.0f)
        , m_slotCount(0)
    {}

    ~ProgressBarTemplate()
    {}

    bool init(const NamedMap<Texture>& textureLib,
              const NamedMap<Color>& colorLib,
              const NamedMap<Rectangle>& rectLib,
              const rapidjson::Value& elem);

    const Texture* getTexture() const
    {
        return m_texture;
    }

    const Color* getColor() const
    {
        return m_color;
    }

    const Rectangle* getRect() const
    {
        return m_rect;
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

    float getSlotCount() const
    {
        return m_slotCount;
    }

private:
    const Texture* m_texture;
    const Color* m_color;
    const Rectangle* m_rect;
    float m_startOffset[Constants::NUM_FLOATS_PER_POSITION];
    float m_barWidth;
    int m_slotCount;
};

} // end of namespace bot

#endif

