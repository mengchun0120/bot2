#ifndef INCLUDE_BOT_TILE
#define INCLUDE_BOT_TILE

#include "misc/bot_time_utils.h"
#include "opengl/bot_color.h"
#include "gameobj/bot_game_object.h"

namespace bot {

class TileTemplate;

class Tile : public GameObject {
public:
    Tile(const TileTemplate *tileTemplate, int level,
         float x, float y) noexcept(false);

    virtual ~Tile()
    {}

    virtual void present();

    virtual void update(float delta, GameScreen &screen);

    int getHP() const
    {
        return m_hp;
    }

    bool addHP(float deltaHP);

    void setPos(float x, float y)
    {
        m_pos[0] = x;
        m_pos[1] = y;
    }

    virtual void onDeath(GameScreen &screen);

protected:
    float m_maxHP;
    float m_hp;
    Color m_mask;
    TimePoint m_deathTime;
};

} // end of namespace bot

#endif
