#include <algorithm>
#include <cmath>
#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "misc/bot_math_utils.h"
#include "geometry/bot_rectangle.h"
#include "gametemplate/bot_base_template.h"
#include "gametemplate/bot_weapon_template.h"
#include "gametemplate/bot_mover_template.h"
#include "gametemplate/bot_missile_template.h"
#include "gametemplate/bot_player_template.h"

namespace bot {

BaseTemplate* parseBaseTemplate(const rapidjson::Value& elem)
{
    const char name[] = "base";

    if (!elem.HasMember(name))
    {
        LOG_ERROR("%s is missing", name);
        return nullptr;
    }

    const rapidjson::Value& obj = elem[name];
    if (!obj.IsObject())
    {
        LOG_ERROR("%s is ill-formatted", name);
        return nullptr;
    }

    BaseTemplate* t = new BaseTemplate();
    if (!t->init(obj))
    {
        delete t;
        return nullptr;
    }

    return t;
}

MissileTemplate* parseMissileTemplate(const rapidjson::Value& elem)
{
    const char name[] = "missile";

    if (!elem.HasMember(name))
    {
        LOG_ERROR("%s is missing", name);
        return nullptr;
    }

    const rapidjson::Value& obj = elem[name];
    if (!obj.IsObject())
    {
        LOG_ERROR("%s is ill-formatted", name);
        return nullptr;
    }

    MissileTemplate* t = new MissileTemplate();
    if (!t->init(obj))
    {
        delete t;
        return nullptr;
    }

    return t;
}

WeaponTemplate* parseWeaponTemplate(const MissileTemplate* missileTemplate,
                                    const rapidjson::Value& elem)
{
    const char name[] = "weapon";

    if (!elem.HasMember(name))
    {
        LOG_ERROR("%s is missing", name);
        return nullptr;
    }

    const rapidjson::Value& obj = elem[name];
    if (!obj.IsObject())
    {
        LOG_ERROR("%s is ill-formatted", name);
        return nullptr;
    }

    WeaponTemplate* t = new WeaponTemplate();
    if (!t->init(missileTemplate, obj))
    {
        delete t;
        return nullptr;
    }

    return t;
}

MoverTemplate* parseMoverTemplate(const rapidjson::Value& elem)
{
    const char name[] = "mover";

    if (!elem.HasMember(name))
    {
        LOG_ERROR("%s is missing", name);
        return nullptr;
    }

    const rapidjson::Value& obj = elem[name];
    if (!obj.IsObject())
    {
        LOG_ERROR("%s is ill-formatted", name);
        return nullptr;
    }

    MoverTemplate* t = new MoverTemplate();
    if (!t->init(obj))
    {
        delete t;
        return nullptr;
    }

    return t;
}

PlayerTemplate::PlayerTemplate()
    : RobotTemplate()
    , m_missileTemplate(nullptr)
    , m_gold(0)
    , m_experience(0L)
    , m_hpLevel(1)
    , m_hpRestoreLevel(1)
    , m_armorLevel(1)
    , m_armorRepairLevel(1)
    , m_powerLevel(1)
    , m_powerRestoreLevel(1)
    , m_missileLevel(1)
    , m_weaponLevel(1)
    , m_moverLevel(1)
{
}

PlayerTemplate::~PlayerTemplate()
{
    if (m_baseTemplate)
    {
        delete m_baseTemplate;
    }

    if (m_weaponTemplate)
    {
        delete m_weaponTemplate;
    }

    if (m_missileTemplate)
    {
        delete m_missileTemplate;
    }

    if (m_moverTemplate)
    {
        delete m_moverTemplate;
    }
}

bool PlayerTemplate::init(const std::string& fileName)
{
    rapidjson::Document doc;
    if (!readJson(doc, fileName.c_str()))
    {
        return false;
    }

    if (!doc.IsObject())
    {
        LOG_ERROR("Invalid format in %s", fileName.c_str());
        return false;
    }

    const rapidjson::Value& elem = doc.GetObject();

    m_baseTemplate = parseBaseTemplate(elem);
    if (!m_baseTemplate)
    {
        return false;
    }

    m_missileTemplate = parseMissileTemplate(elem);
    if (!m_missileTemplate)
    {
        return false;
    }

    m_weaponTemplate = parseWeaponTemplate(m_missileTemplate, elem);
    if (!m_weaponTemplate)
    {
        return false;
    }

    m_moverTemplate = parseMoverTemplate(elem);
    if (!m_moverTemplate)
    {
        return false;
    }

    int gold, experience;
    int hpLevel, hpRestoreLevel;
    int armorLevel, armorRepairLevel;
    int powerLevel, powerRestoreLevel;
    int missileLevel, weaponLevel, moverLevel;
    std::vector<JsonParamPtr> params = {
        jsonParam(gold, "gold"),
        jsonParam(experience, "experience"),
        jsonParam(hpLevel, "hpLevel"),
        jsonParam(hpRestoreLevel, "hpRestoreLevel"),
        jsonParam(armorLevel, "armorLevel"),
        jsonParam(armorRepairLevel, "armorRepairLevel"),
        jsonParam(powerLevel, "powerLevel"),
        jsonParam(powerRestoreLevel, "powerRestoreLevel"),
        jsonParam(missileLevel, "missileLevel"),
        jsonParam(weaponLevel, "weaponLevel"),
        jsonParam(moverLevel, "moverLevel")
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    bool success = setGold(gold) &&
                   setExperience(experience) &&
                   setHPLevel(hpLevel) &&
                   setHPRestoreLevel(hpRestoreLevel) &&
                   setArmorLevel(armorLevel) &&
                   setArmorRepairLevel(armorRepairLevel) &&
                   setPowerLevel(powerLevel) &&
                   setPowerRestoreLevel(powerRestoreLevel) &&
                   setMissileLevel(missileLevel) &&
                   setWeaponLevel(weaponLevel) &&
                   setMoverLevel(moverLevel);
    if (!success)
    {
        return false;
    }

    configCoverCollideBreath();

    return true;
}

bool PlayerTemplate::setGold(int gold)
{
    if (gold < 0)
    {
        LOG_ERROR("Invalid gold %d", gold);
        return false;
    }

    m_gold = gold;
    return true;
}

bool PlayerTemplate::setExperience(float experience)
{
    if (experience < 0)
    {
        LOG_ERROR("Invalid experience %d", experience);
        return false;
    }

    m_experience = experience;
    return true;
}

bool PlayerTemplate::setHPLevel(int level)
{
    if (level < 0)
    {
        LOG_ERROR("Invalid hp-level %d", level);
        return false;
    }

    m_hpLevel = level;
    return true;
}

bool PlayerTemplate::setHPRestoreLevel(int level)
{
    if (level < 0)
    {
        LOG_ERROR("Invalid hp-restore-level %d", level);
        return false;
    }

    m_hpRestoreLevel = level;
    return true;
}

bool PlayerTemplate::setArmorLevel(int level)
{
    if (level < 0)
    {
        LOG_ERROR("Invalid armor-level %d", level);
        return false;
    }

    m_armorLevel = level;
    return true;
}

bool PlayerTemplate::setArmorRepairLevel(int level)
{
    if (level < 0)
    {
        LOG_ERROR("Invalid armor-repair-level %d", level);
        return false;
    }

    m_armorRepairLevel = level;
    return true;
}

bool PlayerTemplate::setPowerLevel(int level)
{
    if (level < 0)
    {
        LOG_ERROR("Invalid power-level %d", level);
        return false;
    }

    m_powerLevel = level;
    return true;
}

bool PlayerTemplate::setPowerRestoreLevel(int level)
{
    if (level < 0)
    {
        LOG_ERROR("Invalid power-restore-level %d", level);
        return false;
    }

    m_powerRestoreLevel = level;
    return true;
}

bool PlayerTemplate::setMissileLevel(int level)
{
    if (level < 0)
    {
        LOG_ERROR("Invalid missile-level %d", level);
        return false;
    }

    m_missileLevel = level;
    return true;
}

bool PlayerTemplate::setWeaponLevel(int level)
{
    if (level < 0)
    {
        LOG_ERROR("Invalid weapon-level %d", level);
        return false;
    }

    m_weaponLevel = level;
    return true;
}

bool PlayerTemplate::setMoverLevel(int level)
{
    if (level < 0)
    {
        LOG_ERROR("Invalid mover-level %d", level);
        return false;
    }

    m_moverLevel = level;
    return true;
}

void PlayerTemplate::configCoverCollideBreath()
{
    if (!m_baseTemplate)
    {
        return;
    }

    const Rectangle* rect = m_baseTemplate->getRect();
    m_coverBreath = std::max(rect->width(), rect->height()) / 2.0f;
    m_collideBreath = m_coverBreath * sqrt(2.0) / 2.0;

    if (m_weaponTemplate)
    {
        rect = m_weaponTemplate->getRect();
        float d = dist(0.0f, 0.0f, m_baseTemplate->getWeaponPosX(),
                       m_baseTemplate->getWeaponPosY());
        float breath = d + std::max(rect->width(), rect->height()) / 2.0f;
        if (breath > m_coverBreath)
        {
            m_coverBreath = breath;
        }
    }

    if (m_moverTemplate)
    {
        rect = m_moverTemplate->getRect();
        float d = dist(0.0f, 0.0f, m_baseTemplate->getMoverPosX(),
                       m_baseTemplate->getMoverPosY());
        float breath = d + std::max(rect->width(), rect->height()) / 2.0f;
        if (breath > m_coverBreath)
        {
            m_coverBreath = breath;
        }
    }
}

} // end of namespace bot
