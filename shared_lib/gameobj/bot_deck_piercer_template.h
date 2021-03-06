#ifndef INCLUDE_BOT_DECK_PIERCER_TEMPLATE
#define INCLUDE_BOT_DECK_PIERCER_TEMPLATE

#include "opengl/bot_color.h"
#include "gameobj/bot_missile_template.h"

namespace bot {

class DeckPiercerTemplate: public MissileTemplate {
public:
    DeckPiercerTemplate();

    virtual ~DeckPiercerTemplate()
    {}

    bool init(const rapidjson::Value &elem);

    const Color &getPassThroughMask() const
    {
        return m_passThroughMask;
    }

    bool getExplodeOnDeath() const
    {
        return m_explodeOnDeath;
    }

    float getExplodeBreath() const
    {
        return m_explodeBreath;
    }

protected:
    Color m_passThroughMask;
    bool m_explodeOnDeath;
    float m_explodeBreath;
};

} // end of namespace bot

#endif

