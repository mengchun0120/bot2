#ifndef INCLUDE_BOT_TILE
#define INCLUDE_BOT_TILE

#include "misc/bot_constants.h"
#include "gametemplate/bot_tile_template.h"
#include "gameobj/bot_game_object.h"

namespace bot {

class TileTemplate;

class Tile : public GameObject {
public:
    Tile(const TileTemplate* tileTemplate);

    virtual ~Tile();

    const TileTemplate* getTemplate() const
    {
        return static_cast<const TileTemplate*>(m_template);
    }

    virtual void present(Graphics& g);

    virtual void update(float delta, GameScreen& screen);

    void setPos(float x, float y);

    int getHP() const
    {
        return m_hp;
    }

    bool addHP(int deltaHP);

protected:
    int m_hp;
};

} // end of namespace bot

#endif
