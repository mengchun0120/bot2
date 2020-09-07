#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "structure/bot_named_map.h"
#include "opengl/bot_texture.h"
#include "opengl/bot_color.h"
#include "geometry/bot_rectangle.h"
#include "gametemplate/bot_tile_template.h"

namespace bot {

TileTemplate* TileTemplate::Parser::create(const std::string& name, const rapidjson::Value& elem)
{
    TileTemplate* t = new TileTemplate();
    if (!t->init(m_textureLib, m_rectLib, elem))
    {
        delete t;
        return nullptr;
    }
    return t;
}

TileTemplate::TileTemplate()
    : GameObjectTemplate(GAME_OBJ_TYPE_TILE)
    , m_texture(nullptr)
    , m_rect(nullptr)
    , m_hp(0)
{
}

bool TileTemplate::init(const NamedMap<Texture>& textureLib, const NamedMap<Rectangle>& rectLib,
                        const rapidjson::Value& elem)
{
    std::string textureName, rectName, colorName;
    bool indestructable = false;

    std::vector<JsonParseParam> params =
    {
        {&textureName,        "texture",        JSONTYPE_STRING},
        {&rectName,           "rect",           JSONTYPE_STRING},
        {&m_coverBreathX,     "coverBreathX",   JSONTYPE_FLOAT},
        {&m_coverBreathY,     "coverBreathY",   JSONTYPE_FLOAT},
        {&m_collideBreathX,   "collideBreathX", JSONTYPE_FLOAT},
        {&m_collideBreathY,   "collideBreathY", JSONTYPE_FLOAT},
        {&m_hp,               "hp",             JSONTYPE_INT},
        {&indestructable,     "indestructable", JSONTYPE_BOOL}
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    m_texture = textureLib.search(textureName);
    if (!m_texture)
    {
        LOG_ERROR("Failed to find texture %s", textureName.c_str());
        return false;
    }

    m_rect = rectLib.search(rectName);
    if (!m_rect)
    {
        LOG_ERROR("Failed to find rectangle %s", rectName.c_str());
        return false;
    }

    if (indestructable)
    {
        setFlag(GAME_OBJ_FLAG_INDESTRUCTABLE);
    }
    else
    {
        clearFlag(GAME_OBJ_FLAG_INDESTRUCTABLE);
    }

    return true;
}

} // end of namespace bot
