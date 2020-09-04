#ifndef INCLUDE_BOT_GOODIE_TEMPLATE
#define INCLUDE_BOT_GOODIE_TEMPLATE

#include <string>
#include <rapidjson/document.h>
#include "gameobj/bot_goodie_type.h"
#include "gametemplate/bot_game_object_template.h"

namespace bot {

template <typename T> class NamedMap;
class Rectangle;
class Texture;
class Color;
class ProgressRing;

class GoodieTemplate : public GameObjectTemplate {
public:
    class Parser {
    public:
        Parser(const NamedMap<Rectangle>& rectLib, const NamedMap<Texture>& textureLib,
               const NamedMap<Color>& colorLib, const NamedMap<ProgressRing>& ringLib)
            : m_rectLib(rectLib)
            , m_textureLib(textureLib)
            , m_colorLib(colorLib)
            , m_ringLib(ringLib)
        {}

        ~Parser()
        {}

        GoodieTemplate* create(const std::string& name, const rapidjson::Value& elem);

    private:
        const NamedMap<Rectangle>& m_rectLib;
        const NamedMap<Texture>& m_textureLib;
        const NamedMap<Color>& m_colorLib;
        const NamedMap<ProgressRing>& m_ringLib;
    };

    GoodieTemplate()
        : GameObjectTemplate(GAME_OBJ_TYPE_GOODIE)
        , m_goodieType(GOODIE_UNKNOWN)
        , m_rect(nullptr)
        , m_texture(nullptr)
        , m_ring(nullptr)
        , m_duration(0.0f)
    {}

    virtual ~GoodieTemplate()
    {}

    bool init(const std::string& name, const NamedMap<Rectangle>& rectLib,
              const NamedMap<Texture>& textureLib, const NamedMap<Color>& colorLib,
              const NamedMap<ProgressRing>& ringLib, const rapidjson::Value& elem);

    GoodieType getGoodieType() const
    {
        return m_goodieType;
    }

    void setGoodieType(GoodieType goodieType)
    {
        m_goodieType = goodieType;
    }

    const Rectangle* getRect() const
    {
        return m_rect;
    }

    void setRect(const Rectangle* rect)
    {
        m_rect = rect;
    }

    const Rectangle* getEffectRect() const
    {
        return m_effectRect;
    }

    void setEffectRect(const Rectangle* rect)
    {
        m_effectRect = rect;
    }

    const Texture* getTexture() const
    {
        return m_texture;
    }

    void setTexture(const Texture* texture)
    {
        m_texture = texture;
    }

    const ProgressRing* getProgressRing() const
    {
        return m_ring;
    }

    void setProgressRing(const ProgressRing* ring)
    {
        m_ring = ring;
    }

    float getDuration() const
    {
        return m_duration;
    }

    void setDuration(float duration)
    {
        m_duration = duration;
    }

    bool isEffect() const
    {
        return m_duration > 0.0f;
    }

    float getWeight() const
    {
        return m_weight;
    }

    void setWeight(float weight)
    {
        m_weight = weight;
    }

private:
    GoodieType m_goodieType;
    const Rectangle* m_rect;
    const Rectangle* m_effectRect;
    const Texture* m_texture;
    const ProgressRing* m_ring;
    float m_duration;
    float m_weight;
};

} // end of namespace bot

#endif