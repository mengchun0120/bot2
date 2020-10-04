#ifndef INCLUDE_BOT_SINGLE_UNIT_TEMPLATE
#define INCLUDE_BOT_SINGLE_UNIT_TEMPLATE

#include <rapidjson/document.h>

namespace bot {

template <typename T> class NamedMap;
class Texture;
class Rectangle;

class SingleUnitTemplate {
public:
    SingleUnitTemplate()
        : m_texture(nullptr)
        , m_rect(nullptr)
    {}

    virtual ~SingleUnitTemplate()
    {}

    bool init(const NamedMap<Texture>& textureLib,
              const NamedMap<Rectangle>& rectLib,
              const rapidjson::Value& elem);

    const Texture* getTexture() const
    {
        return m_texture;
    }

    void setTexture(const Texture* texture)
    {
        m_texture = texture;
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
    const Texture* m_texture;
    const Rectangle* m_rect;
};

} // end of namespace bot

#endif

