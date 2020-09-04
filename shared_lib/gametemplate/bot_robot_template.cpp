#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "structure/bot_named_map.h"
#include "opengl/bot_texture.h"
#include "opengl/bot_color.h"
#include "geometry/bot_rectangle.h"
#include "gametemplate/bot_missile_template.h"
#include "gametemplate/bot_robot_template.h"

namespace bot {

RobotTemplate::RobotTemplate()
    : GameObjectTemplate(GAME_OBJ_TYPE_ROBOT)
{
    initAbilityTemplates();
}

RobotTemplate::~RobotTemplate()
{
    for (int i = 0; i < NUM_OF_ABILITIES; ++i)
    {
        if (!m_abilityTemplates[i])
        {
            delete m_abilityTemplates[i];
        }
    }
}

bool RobotTemplate::init(const NamedMap<Texture>& textureLib, const NamedMap<Rectangle>& rectLib,
                         const NamedMap<Color>& colorLib, const NamedMap<MissileTemplate>& missileLib,
                         const rapidjson::Value& elem)
{
    if (!parseBaseAttributes(elem))
    {
        return false;
    }

    if (!parseComponents(textureLib, rectLib, colorLib, elem))
    {
        return false;
    }

    if (!parseMoveAbility(elem))
    {
        return false;
    }

    if (!parseShootAbility(missileLib, elem))
    {
        return false;
    }

    return true;
}

void RobotTemplate::setNumComponents(int numComponents)
{
    m_components.resize(numComponents);
    initComponents();
}

void RobotTemplate::initComponents()
{
    int numComponents = getNumComponents();

    for (int i = 0; i < numComponents; ++i) {
        m_components[i].m_index = i;
        m_components[i].m_texture = nullptr;
        m_components[i].m_pos[0] = 0.0f;
        m_components[i].m_pos[1] = 0.0f;
    }
}

void RobotTemplate::initAbilityTemplates()
{
    for (int i = 0; i < NUM_OF_ABILITIES; ++i) {
        m_abilityTemplates[i] = nullptr;
        m_attachComponents[i] = nullptr;
    }
}

void RobotTemplate::setMoveAbilityTemplate(float speed)
{
    MoveAbilityTemplate* moveAbility = new MoveAbilityTemplate(speed);
    m_abilityTemplates[ABILITY_MOVE] = moveAbility;
}

void RobotTemplate::setShootAbilityTemplate(float shootInterval, float shootPosX, float shootPosY,
                                            const MissileTemplate* missileTemplate)
{
    ShootAbilityTemplate* shootAbility = new ShootAbilityTemplate(shootInterval, shootPosX,
                                                                  shootPosY, missileTemplate);

    m_abilityTemplates[ABILITY_SHOOT] = shootAbility;
}

bool RobotTemplate::parseBaseAttributes(const rapidjson::Value& elem)
{
    std::vector<JsonParseParam> robotParams =
    {
        {&m_coverBreathX,    "coverBreathX",    JSONTYPE_FLOAT},
        {&m_coverBreathY,    "coverBreathY",    JSONTYPE_FLOAT},
        {&m_collideBreathX,  "collideBreathX",  JSONTYPE_FLOAT},
        {&m_collideBreathY,  "collideBreathY",  JSONTYPE_FLOAT},
        {&m_hp,              "hp",              JSONTYPE_INT},
        {&m_goodieSpawnProb, "goodieSpawnProb", JSONTYPE_FLOAT}
    };

    if (!parseJson(robotParams, elem))
    {
        return false;
    }

    return true;
}

bool RobotTemplate::parseComponents(const NamedMap<Texture>& textureLib, const NamedMap<Rectangle>& rectLib,
                                    const NamedMap<Color>& colorLib, const rapidjson::Value& elem)
{
    if (!elem.HasMember("components"))
    {
        LOG_ERROR("Components missing");
        return false;
    }

    if (!elem["components"].IsArray())
    {
        LOG_ERROR("Components must be an array");
        return false;
    }

    const rapidjson::Value& components = elem["components"];
    int numComponents = components.Capacity();
    std::string textureName, rectName, colorName;
    float componentX = 0.0f, componentY = 0.0f;

    std::vector<JsonParseParam> componentParams =
    {
        {&textureName, "texture",    JSONTYPE_STRING},
        {&rectName,    "rect",       JSONTYPE_STRING},
        {&colorName,   "color",      JSONTYPE_STRING},
        {&componentX,  "componentX", JSONTYPE_FLOAT},
        {&componentY,  "componentY", JSONTYPE_FLOAT}
    };

    m_components.resize(numComponents);
    for (int i = 0; i < numComponents; ++i)
    {
        if (!parseJson(componentParams, components[i]))
        {
            return false;
        }

        const Texture* texture = textureLib.search(textureName);
        if (!texture)
        {
            LOG_ERROR("Failed to find texture %s", textureName.c_str());
            return false;
        }

        const Rectangle* rect = rectLib.search(rectName);
        if (!rect)
        {
            LOG_ERROR("Failed to find rect %s", rectName.c_str());
            return false;
        }

        const Color* color = colorLib.search(colorName);
        if (!color)
        {
            LOG_ERROR("Failed to find color %s", colorName.c_str());
            return false;
        }

        m_components[i].m_index = i;
        m_components[i].m_texture = texture;
        m_components[i].m_rect = rect;
        m_components[i].m_color = color;
        m_components[i].m_pos[0] = componentX;
        m_components[i].m_pos[1] = componentY;
    }

    return true;
}

bool RobotTemplate::parseMoveAbility(const rapidjson::Value& elem)
{
    bool movable = false;
    float moveSpeed = 0.0f;
    int moveComponentIdx = 0;

    std::vector<JsonParseParam> params =
    {
        {&movable,            "movable",            JSONTYPE_BOOL},
        {&moveSpeed,          "moveSpeed",          JSONTYPE_FLOAT},
        {&moveComponentIdx,   "moveComponentIdx",   JSONTYPE_INT}
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    if (!movable)
    {
        return true;
    }

    if (moveComponentIdx < 0 || moveComponentIdx >= getNumComponents())
    {
        LOG_ERROR("moveComponentIdx out of range");
        return false;
    }

    MoveAbilityTemplate* moveAbility = new MoveAbilityTemplate(moveSpeed);
    m_abilityTemplates[ABILITY_MOVE] = moveAbility;
    m_attachComponents[ABILITY_MOVE] = &m_components[moveComponentIdx];

    return true;
}

bool RobotTemplate::parseShootAbility(const NamedMap<MissileTemplate>& missileLib, const rapidjson::Value& elem)
{
    bool shootable = false;
    float shootInterval = 0.0f, shootPosX = 0.0f, shootPosY = 0.0f;
    int shootComponentIdx = 0;
    std::string missileName;

    std::vector<JsonParseParam> params =
    {
        {&shootable,          "shootable",          JSONTYPE_BOOL},
        {&shootInterval,      "shootInterval",      JSONTYPE_FLOAT},
        {&shootPosX,          "shootPosX",          JSONTYPE_FLOAT},
        {&shootPosY,          "shootPosY",          JSONTYPE_FLOAT},
        {&shootComponentIdx,  "shootComponentIdx",  JSONTYPE_INT},
        {&missileName,        "missile",            JSONTYPE_STRING}
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    if (!shootable)
    {
        return true;
    }

    if (shootComponentIdx < 0 || shootComponentIdx >= getNumComponents())
    {
        LOG_ERROR("shootComponentIdx out of range");
        return false;
    }

    const MissileTemplate* missile = missileLib.search(missileName);
    if (!missile)
    {
        LOG_ERROR("Couldn't find missile %s", missileName.c_str());
        return false;
    }

    ShootAbilityTemplate* shootAbility = new ShootAbilityTemplate(shootInterval, shootPosX,
                                                                  shootPosY, missile);
    m_abilityTemplates[ABILITY_SHOOT] = shootAbility;
    m_attachComponents[ABILITY_SHOOT] = &m_components[shootComponentIdx];

    return true;
}

} // end of namespace bot
