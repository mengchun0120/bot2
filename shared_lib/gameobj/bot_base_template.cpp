#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "gameutil/bot_game_lib.h"

namespace bot {

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
    , m_hpColor(nullptr)
{}

bool BaseTemplate::init(const rapidjson::Value &elem)
{
    if (!SingleUnitTemplate::init(elem))
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
    std::string hpColorName;
    std::vector<JsonParamPtr> params = {
        jsonParam(hp, "hp"),
        jsonParam(hpPerLevel, "hpPerLevel", false),
        jsonParam(hpRestoreRate, "hpRestoreRate", false),
        jsonParam(hpRestoreRatePerLevel, "hpRestoreRatePerLevel", false),
        jsonParam(armor, "armor", false),
        jsonParam(armorPerLevel, "armorPerLevel", false),
        jsonParam(armorRepairRate,  "armorRepairRate", false),
        jsonParam(armorRepairRatePerLevel,  "armorRepairRatePerLevel", false),
        jsonParam(power, "power", false),
        jsonParam(powerPerLevel, "powerPerLevel", false),
        jsonParam(powerRestoreRate, "powerRestoreRate", false),
        jsonParam(powerRestoreRatePerLevel, "powerRestoreRatePerLevel", false),
        jsonParam(weaponPos, "weaponPos"),
        jsonParam(moverPos, "moverPos"),
        jsonParam(hpColorName, "hpColor")
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

    const GameLib &lib = GameLib::getInstance();

    m_hpColor = lib.getColor(hpColorName);
    if (!m_hpColor)
    {
        LOG_ERROR("Failed to find color %s", hpColorName.c_str());
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

