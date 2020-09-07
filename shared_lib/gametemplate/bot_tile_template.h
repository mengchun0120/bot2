#ifndef INCLUDE_BOT_TILE_TEMPLATE
#define INCLUDE_BOT_TILE_TEMPLATE

#include <string>
#include <rapidjson/document.h>
#include "gametemplate/bot_game_object_template.h"

namespace bot {

template <typename T> class NamedMap;
class Texture;
class Rectangle;

class TileTemplate: public GameObjectTemplate {
public:
    class Parser {
    public:
        Parser(const NamedMap<Texture>& textureLib, const NamedMap<Rectangle>& rectLib)
            : m_textureLib(textureLib)
            , m_rectLib(rectLib)
        {}

        ~Parser()
        {}

        TileTemplate* create(const std::string& name, const rapidjson::Value& elem);

    private:
        const NamedMap<Texture>& m_textureLib;
        const NamedMap<Rectangle>& m_rectLib;
    };

    TileTemplate();

    virtual ~TileTemplate()
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

    int getHP() const
    {
        return m_hp;
    }

protected:
    const Texture* m_texture;
    const Rectangle* m_rect;
    int m_hp;
};

} // end of namespace bot

#endif
