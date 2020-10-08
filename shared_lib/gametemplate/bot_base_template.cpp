#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "structure/bot_named_map.h"
#include "opengl/bot_texture.h"
#include "geometry/bot_rectangle.h"
#include "gametemplate/bot_base_template.h"

namespace bot {

BaseTemplate* BaseTemplate::Parser::create(std::string& name,
                                           const rapidjson::Value& elem)
{
    BaseTemplate* t = new BaseTemplate();
    if (!t->init(m_textureLib, m_rectLib, elem))
    {
        delete t;
        return nullptr;
    }

    return t;
}

BaseTemplate::BaseTemplate()
    : m_hp(0.0f)
    , m_hpPerLevel(0.0f)
    , m_hpRestoreRate(0.0f)
    , m_hpRestoreRatePerLevel(0.0f)
    , m_armor(0.0f)
    , m_armorPerLevel(0.0f)
    , m_armorRepairRate(0.0f)
    , m_armorRepairRatePerLevel(0.0f)
    , m_power(0.0f)
    , m_powerPerLevel(0.0f)
    , m_powerRestoreRate(0.0f)
    , m_powerRestoreRatePerLevel(0.0f)
{}

bool BaseTemplate::init(const NamedMap<Texture>& textureLib,
                        const NamedMap<Rectangle>& rectLib,
                        const rapidjson::Value& elem)
{
    if (!SingleUnitTemplate::init(textureLib, rectLib, elem))
    {
        LOG_ERROR("Failed to initialize SingleUnitTemplate");
        return false;
    }

    float hp, hpPerLevel = 0.0f;
    float hpRestoreRate = 0.0f, hpRestoreRatePerLevel = 0.0f;
    float armor = 0.0f, armorPerLevel = 0.0f;
    float armorRepairRate = 0.0f, armorRepairRatePerLevel = 0.0f;
    float power = 0.0f, powerPerLevel = 0.0f;
    float powerRestoreRate = 0.0f, powerRestoreRatePerLevel = 0.0f;
    std::vector<float> weaponPos, moverPos;
    std::vector<JsonParseParam> params = {
        {
            &hp,
            "hp",
            JSONTYPE_FLOAT
        },
        {
            &hpPerLevel,
            "hpPerLevel",
            JSONTYPE_FLOAT,
            false},
        {
            &hpRestoreRate,
            "hpRestoreRate",
            JSONTYPE_FLOAT,
            false
        },
        {
            &hpRestoreRatePerLevel,
            "hpRestoreRatePerLevel",
            JSONTYPE_FLOAT,
            false
        },
        {
            &armor,
            "armor",
            JSONTYPE_FLOAT,
            false
        },
        {
            &armorPerLevel,
            "armorPerLevel",
            JSONTYPE_FLOAT,
            false
        },
        {
            &armorRepairRate,
            "armorRepairRate",
            JSONTYPE_FLOAT,
            false
        },
        {
            &armorRepairRatePerLevel,
            "armorRepairRatePerLevel",
            JSONTYPE_FLOAT,
            false
        },
        {
            &power,
            "power",
            JSONTYPE_FLOAT,
            false
        },
        {
            &powerPerLevel,
            "powerPerLevel",
            JSONTYPE_FLOAT,
            false
        },
        {
            &powerRestoreRate,
            "powerRestoreRate",
            JSONTYPE_FLOAT,
            false
        },
        {
            &powerRestoreRatePerLevel,
            "powerRestoreRatePerLevel",
            JSONTYPE_FLOAT,
            false
        },
        {
            &weaponPos,
            "weaponPos",
            JSONTYPE_FLOAT_ARRAY
        },
        {
            &moverPos,
            "moverPos",
            JSONTYPE_FLOAT_ARRAY
        }
    };

    if (!parseJson(params, elem))
    {
        LOG_ERROR("parseJson failed");
        return false;
    }

    if (weaponPos.size() != Constants::NUM_FLOATS_PER_POSITION)
    {
        LOG_ERROR("Invalid weapon position");
        return false;
    }

    if (moverPos.size() != Constants::NUM_FLOATS_PER_POSITION)
    {
        LOG_ERROR("Invalid mover position");
        return false;
    }

    bool success = setHP(hp) &&
                   setHPPerLevel(hpPerLevel) &&
                   setHPRestoreRate(hpRestoreRate) &&
                   setHPRestoreRatePerLevel(hpRestoreRatePerLevel) &&
                   setArmor(armor) &&
                   setArmorPerLevel(armorPerLevel) &&
                   setArmorRepairRate(armorRepairRate) &&
                   setArmorRepairRatePerLevel(armorRepairRatePerLevel) &&
                   setPower(power) &&
                   setPowerPerLevel(powerPerLevel) &&
                   setPowerRestoreRate(powerRestoreRate) &&
                   setPowerRestoreRatePerLevel(powerRestoreRatePerLevel);
    if (!success)
    {
        LOG_ERROR("Failed to initialize param");
        return false;
    }

    setWeaponPos(weaponPos[0], weaponPos[1]);
    setMoverPos(moverPos[0], moverPos[1]);

    return true;
}

bool BaseTemplate::setHP(float hp)
{
    if (hp < 0.0f)
    {
        LOG_ERROR("Invalid hp %f", hp);
        return false;
    }

    m_hp = hp;
    return true;
}

bool BaseTemplate::setHPPerLevel(float hpPerLevel)
{
    if (hpPerLevel < 0.0f)
    {
        LOG_ERROR("Invalid hp-per-level %f", hpPerLevel);
        return false;
    }

    m_hpPerLevel = hpPerLevel;
    return true;
}

bool BaseTemplate::setHPRestoreRate(float rate)
{
    if (rate < 0.0f)
    {
        LOG_ERROR("Invalid hp-restore-rate %f", rate);
        return false;
    }

    m_hpRestoreRate = rate;
    return true;
}

bool BaseTemplate::setHPRestoreRatePerLevel(float ratePerLevel)
{
    if (ratePerLevel < 0.0f)
    {
        LOG_ERROR("Invalid hp-restore-rate-per-level %f", ratePerLevel);
        return false;
    }

    m_hpRestoreRatePerLevel = ratePerLevel;
    return true;
}

bool BaseTemplate::setArmor(float armor)
{
    if (armor < 0.0f)
    {
        LOG_ERROR("Invalid armor %f", armor);
        return false;
    }

    m_armor = armor;
    return true;
}

bool BaseTemplate::setArmorPerLevel(float armorPerLevel)
{
    if (armorPerLevel < 0.0f)
    {
        LOG_ERROR("Invalid armor-per-level %f", armorPerLevel);
        return false;
    }

    m_armorPerLevel = armorPerLevel;
    return true;
}

bool BaseTemplate::setArmorRepairRate(float rate)
{
    if (rate < 0.0f)
    {
        LOG_ERROR("Invalid armor-repair-rate %f", rate);
        return false;
    }

    m_armorRepairRate = rate;
    return true;
}

bool BaseTemplate::setArmorRepairRatePerLevel(float ratePerLevel)
{
    if (ratePerLevel < 0.0f)
    {
        LOG_ERROR("Invalid armor-repair-rate-per-level %f", ratePerLevel);
        return false;
    }

    m_armorRepairRatePerLevel = ratePerLevel;
    return true;
}

bool BaseTemplate::setPower(float power)
{
    if (power < 0.0f)
    {
        LOG_ERROR("Invalid power %f", power);
        return false;
    }

    m_power = power;
    return true;
}

bool BaseTemplate::setPowerPerLevel(float powerPerLevel)
{
    if (powerPerLevel < 0.0f)
    {
        LOG_ERROR("Invalid power-per-level %f", powerPerLevel);
        return false;
    }

    m_powerPerLevel = powerPerLevel;
    return true;
}

bool BaseTemplate::setPowerRestoreRate(float rate)
{
    if (rate < 0.0f)
    {
        LOG_ERROR("Invalid power-restore-rate %f", rate);
        return false;
    }

    m_powerRestoreRate = rate;
    return true;
}

bool BaseTemplate::setPowerRestoreRatePerLevel(float ratePerLevel)
{
    if (ratePerLevel < 0.0f)
    {
        LOG_ERROR("Invalid power-restore-rate-per-level %f", ratePerLevel);
        return false;
    }

    m_powerRestoreRatePerLevel = ratePerLevel;
    return true;
}

} // end of namespace bot

