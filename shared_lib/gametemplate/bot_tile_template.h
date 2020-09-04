#ifndef INCLUDE_BOT_TILE_TEMPLATE
#define INCLUDE_BOT_TILE_TEMPLATE

#include <string>
#include <rapidjson/document.h>
#include "gametemplate/bot_game_object_template.h"

namespace bot {

template <typename T> class NamedMap;
class Texture;
class Rectangle;
class Color;

class TileTemplate: public GameObjectTemplate {
public:
    class Parser {
    public:
        Parser(const NamedMap<Texture>& textureLib, const NamedMap<Rectangle>& rectLib,
               const NamedMap<Color>& colorLib)
            : m_textureLib(textureLib)
            , m_rectLib(rectLib)
            , m_colorLib(colorLib)
        {}

        ~Parser()
        {}

        TileTemplate* create(const std::string& name, const rapidjson::Value& elem);

    private:
        const NamedMap<Texture>& m_textureLib;
        const NamedMap<Rectangle>& m_rectLib;
        const NamedMap<Color>& m_colorLib;
    };

    TileTemplate();

    virtual ~TileTemplate()
    {}

    bool init(const NamedMap<Texture>& textureLib, const NamedMap<Rectangle>& rectLib,
              const NamedMap<Color>& colorLib, const rapidjson::Value& elem);

    const Texture* getTexture() const
    {
        return m_texture;
    }

    const Rectangle* getRect() const
    {
        return m_rect;
    }

    const Color* getColor() const
    {
        return m_color;
    }

    int getHP() const
    {
        return m_hp;
    }

protected:
    const Texture* m_texture;
    const Rectangle* m_rect;
    const Color* m_color;
    int m_hp;
};

} // end of namespace bot

#endif
