#ifndef INCLUDE_BOT_COMPONENT_TEMPLATE
#define INCLUDE_BOT_COMPONENT_TEMPLATE

#include <string>
#include <rapidjson/document.h>

namespace bot {

template <typename T> class NamedMap;
class Texture;
class Rectangle;
class MissileTemplate;

class ComponentTemplate {
public:
    class Parser {
    public:
        Parser(const NamedMap<Texture>& textureLib, const NamedMap<Rectangle>& rectLib,
               const NamedMap<MissileTemplate>& missileLib)
            : m_textureLib(textureLib)
            , m_rectLib(rectLib)
            , m_missileLib(missileLib)
        {}

        ~Parser()
        {}

        ComponentTemplate* create(const std::string& name, const rapidjson::Value& elem);

    private:
        const NamedMap<Texture>& m_textureLib;
        const NamedMap<Rectangle>& m_rectLib;
        const NamedMap<MissileTemplate>& m_missileLib;
    };

    enum Type {
        COMPONENT_BASE,
        COMPONENT_WEAPON,
        COMPONENT_MOVER
    };

    ComponentTemplate(Type type)
        : m_type(type)
        , m_texture(nullptr)
        , m_rect(nullptr)
    {}

    virtual ~ComponentTemplate()
    {}

    bool init(const NamedMap<Texture>& textureLib, const NamedMap<Rectangle>& rectLib,
              const rapidjson::Value& elem);

    const Texture* getTexture() const
    {
        return m_texture;
    }

    const Rectangle* getRect() const
    {
        return m_rect;
    }

protected:
    Type m_type;
    const Texture* m_texture;
    const Rectangle* m_rect;
};

} // end of namespace bot

#endif

