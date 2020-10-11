#ifndef INCLUDE_BOT_TILE_TEMPLATE
#define INCLUDE_BOT_TILE_TEMPLATE

#include <string>
#include "gametemplate/bot_game_object_template.h"
#include "gametemplate/bot_single_unit_template.h"

namespace bot {

class TileTemplate: public GameObjectTemplate, public SingleUnitTemplate {
public:
    class Parser {
    public:
        Parser(const NamedMap<Texture>& textureLib,
               const NamedMap<Rectangle>& rectLib)
            : m_textureLib(textureLib)
            , m_rectLib(rectLib)
        {}

        ~Parser()
        {}

        TileTemplate* create(const std::string& name,
                             const rapidjson::Value& elem);

    private:
        const NamedMap<Texture>& m_textureLib;
        const NamedMap<Rectangle>& m_rectLib;
    };

    TileTemplate();

    virtual ~TileTemplate()
    {}

    bool init(const NamedMap<Texture>& textureLib,
              const NamedMap<Rectangle>& rectLib,
              const rapidjson::Value& elem);

    float getHP(int level) const
    {
        return m_hp + level * m_hpPerLevel;
    }

    bool setHP(float hp);

    bool setHPPerLevel(float hpPerLevel);

protected:
    float m_hp;
    float m_hpPerLevel;
};

} // end of namespace bot

#endif
