#ifndef INCLUDE_BOT_BUTTON_CONFIG
#define INCLUDE_BOT_BUTTON_CONFIG

#include <string>

namespace bot {

template <typename T> class NamedMap;
class Color;
class Texture;

class ButtonConfig {
public:
    ButtonConfig()
        : m_texture(nullptr)
        , m_normalTextColor(nullptr)
        , m_hoverTextColor(nullptr)
        , m_pressTextColor(nullptr)
    {}

    ~ButtonConfig()
    {}

    bool init(const std::string& configFile, const NamedMap<Texture>& textureLib,
              const NamedMap<Color>& colorLib);

    const Texture* getTexture() const
    {
        return m_texture;
    }

    const Color* getNormalTextColor() const
    {
        return m_normalTextColor;
    }

    const Color* getHoverTextColor() const
    {
        return m_hoverTextColor;
    }

    const Color* getPressTextColor() const
    {
        return m_pressTextColor;
    }

private:
    const Texture* m_texture;
    const Color* m_normalTextColor;
    const Color* m_hoverTextColor;
    const Color* m_pressTextColor;
};

} // end of namespace bot

#endif
