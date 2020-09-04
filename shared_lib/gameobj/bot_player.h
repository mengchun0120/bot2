#ifndef INCLUDE_BOT_PLAYER
#define INCLUDE_BOT_PLAYER

#include "gametemplate/bot_player_template.h"
#include "gameobj/bot_goodie_effect.h"
#include "gameobj/bot_robot.h"

namespace bot {

class Goodie;
class GameObjectManager;

class Player : public Robot {
private:
    enum {
        HP_STR_LEN = 11,
        GOLD_STR_LEN = 9
    };

public:
    Player(const PlayerTemplate* playerTemplate);

    virtual ~Player();

    const PlayerTemplate* getTemplate() const
    {
        return static_cast<const PlayerTemplate*>(m_template);
    }

    virtual void present(Graphics& g);

    virtual void update(float delta, GameScreen& screen);

    void consumeGoodie(Goodie* goodie, GameScreen& screen);

    const GoodieEffect* getFirstActiveEffect() const
    {
        return m_firstActiveEffect;
    }

    virtual bool addHP(int deltaHP);

    const char* getHPStr() const
    {
        return m_hpStr;
    }

    int getGoldCount() const
    {
        return m_goldCount;
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

private:
    std::vector<GoodieEffect> m_effects;
    GoodieEffect* m_firstActiveEffect;
    GoodieEffect* m_firstFreeEffect;
    int m_activeEffectCount;
    int m_goldCount;
    long long m_experience;
    float m_experienceMultiplier;
    char m_hpStr[HP_STR_LEN];
    char m_goldStr[GOLD_STR_LEN];
};

} // end of namespace bot

#endif
