#ifndef INCLUDE_BOT_WEAPON_TEMPLATE
#define INCLUDE_BOT_WEAPON_TEMPLATE

#include <string>
#include <vector>
#include "gameobj/bot_fire_point.h"
#include "gametemplate/bot_single_unit_template.h"

namespace bot {

class MissileTemplate;

class WeaponTemplate: public SingleUnitTemplate {
public:
    class Parser {
    public:
        Parser(const NamedMap<Texture>& textureLib,
               const NamedMap<Rectangle>& rectLib,
               const NamedMap<MissileTemplate>& missileLib)
            : m_textureLib(textureLib)
            , m_rectLib(rectLib)
            , m_missileLib(missileLib)
        {}

        ~Parser()
        {}

        WeaponTemplate* create(std::string& name,
                               const rapidjson::Value& elem);

    private:
        const NamedMap<Texture>& m_textureLib;
        const NamedMap<Rectangle>& m_rectLib;
        const NamedMap<MissileTemplate>& m_missileLib;
    };

public:
    WeaponTemplate();

    virtual ~WeaponTemplate()
    {}

    bool init(const NamedMap<Texture>& textureLib,
              const NamedMap<Rectangle>& rectLib,
              const NamedMap<MissileTemplate>& missileLib,
              const rapidjson::Value& elem);

    bool init(const NamedMap<Texture>& textureLib,
              const NamedMap<Rectangle>& rectLib,
              const MissileTemplate* missileTemplate,
              const rapidjson::Value& elem);

    float getFireDuration(int level) const;

    bool setFireDuration(float duration);

    bool setFireDurReductionPerLevel(float reductionPerLevel);

    const MissileTemplate* getMissileTemplate() const
    {
        return m_missileTemplate;
    }

    void setMissileTemplate(const MissileTemplate* missileTemplate)
    {
        m_missileTemplate = missileTemplate;
    }

    int numFirePoints() const
    {
        return static_cast<int>(m_firePoints.size());
    }

    const FirePoint& getFirePoint(int idx) const
    {
        return m_firePoints[idx];
    }

    bool setNumFirePoints(int count);

    FirePoint& getFirePoint(int idx)
    {
        return m_firePoints[idx];
    }

protected:
    float m_fireDuration;
    float m_fireDurReductionPerLevel;
    const MissileTemplate* m_missileTemplate;
    std::vector<FirePoint> m_firePoints;
};

} // end of namespace bot

#endif

