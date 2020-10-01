#ifndef INCLUDE_BOT_TILE
#define INCLUDE_BOT_TILE

#include "gameobj/bot_game_object.h"

namespace bot {

class TileTemplate;

class Tile : public GameObject {
public:
    Tile();

    virtual ~Tile()
    {}

    bool init(const TileTemplate* tileTemplate, int level, float x, float y);

    virtual void present(Graphics& g);

    virtual void update(float delta, GameScreen& screen);

    int getHP() const
    {
        return m_hp;
    }

    bool addHP(float deltaHP);

protected:
    float m_hp;
    float m_maxHP;
};

} // end of namespace bot

#endif
