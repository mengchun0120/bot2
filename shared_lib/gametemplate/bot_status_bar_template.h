#ifndef INCLUDE_BOT_STATUS_BAR_TEMPLATE
#define INCLUDE_BOT_STATUS_BAR_TEMPLATE

#include <rapidjson/document.h>
#include "opengl/bot_text_size.h"
#include "opengl/bot_color.h"
#include "geometry/bot_rectangle.h"

namespace bot {

template <typename T> class NamedMap;
class Texture;

class StatusBarTemplate {
public:
    class Parser {
    public:
        Parser(const NamedMap<Texture>& textureLib)
            : m_textureLib(textureLib)
        {}

        ~Parser()
        {}

        StatusBarTemplate* create(const std::string& name,
                                  const rapidjson::Value& elem);

    private:
        const NamedMap<Texture>& m_textureLib;
    };

    StatusBarTemplate();

    ~StatusBarTemplate()
    {}

    bool init(const NamedMap<Texture>& textureLib,
              const rapidjson::Value& elem);

    const Texture* getTexture() const
    {
        return m_texture;
    }

    const Rectangle& getRect() const
    {
        return m_rect;
    }

    float getTextureTextSpacing() const
    {
        return m_textureTextSpacing;
    }

    const Color& getTextColor() const
    {
        return m_textColor;
    }

    int getTextLen() const
    {
        return m_textLen;
    }

    float getWidth() const
    {
        return m_width;
    }

    float getHeight() const
    {
        return m_height;
    }

    TextSize getTextSize() const
    {
        return m_textSize;
    }

private:
    const Texture* m_texture;
    Rectangle m_rect;
    float m_textureTextSpacing;
    Color m_textColor;
    int m_textLen;
    float m_width;
    float m_height;
    TextSize m_textSize;
};

} // end of namespace bot

#endif

