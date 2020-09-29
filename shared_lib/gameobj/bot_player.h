#ifndef INCLUDE_BOT_PLAYER
#define INCLUDE_BOT_PLAYER

#include <vector>
#include "gameobj/bot_goodie_effect.h"
#include "gameobj/bot_robot.h"

namespace bot {

class Goodie;

class Player : public Robot {
    enum {
        GOLD_STR_LEN = 30
    };

public:
    Player();

    virtual ~Player();

    bool init(const PlayerTemplate* playerTemplate, float x, float y,
              float directionX, float directionY);

    virtual void present(Graphics& g);

    virtual void update(float delta, GameScreen& screen);

    void consumeGoodie(Goodie* goodie, GameScreen& screen);

    const GoodieEffect* getFirstActiveEffect() const
    {
        return m_firstActiveEffect;
    }

    virtual bool addHP(int deltaHP);

    int getGold() const
    {
        return m_gold;
    }

    const char* getGoldStr() const
    {
        return m_goldStr;
    }

private:
    void initEffects();

    void updateEffects();

    void applyInstantaneousEffect(Goodie* goodie);

    void applyNonInstantaneousEffect(Goodie* goodie);

    bool addEffect(Goodie* goodie);

    void expireEffect(GoodieEffect* goodie);

    void resetGoldStr();

private:
    std::vector<GoodieEffect> m_effects;
    GoodieEffect* m_firstActiveEffect;
    GoodieEffect* m_firstFreeEffect;
    int m_activeEffectCount;
    int m_experience;
    float m_experienceMultiplier;
    int m_gold;
    char m_goldStr[GOLD_STR_LEN];
};

} // end of namespace bot

#endif
