#include <GL/glew.h>
#include "misc/bot_json_utils.h"
#include "geometry/bot_rectangle.h"

namespace bot {

Rectangle* Rectangle::Parser::create(const std::string& name, const rapidjson::Value& elem)
{
    Rectangle* rect = new Rectangle();
    if (!rect->init(elem))
    {
        delete rect;
        return nullptr;
    }

    return rect;
}

Rectangle::Rectangle()
: Polygon()
, m_width(0)
, m_height(0)
{
}

Rectangle::~Rectangle()
{
}

bool Rectangle::init(const rapidjson::Value& elem)
{
    std::vector<JsonParseParam> params =
    {
        {&m_width,  "width",  JSONTYPE_FLOAT},
        {&m_height, "height", JSONTYPE_FLOAT}
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    if (!init(m_width, m_height, true))
    {
        LOG_ERROR("Failed to initialize rectangle");
        return false;
    }

    return true;
}

bool Rectangle::init(float width0, float height0, bool hasTexCoord)
{
    float halfWidth = width0 / 2.0f;
    float halfHeight = height0 / 2.0f;
    const unsigned int NUM_VERTICES = 6;

    if(!hasTexCoord) {
        float vertices[] = {
            0.0f, 0.0f,
            halfWidth, halfHeight,
            -halfWidth, halfHeight,
            -halfWidth, -halfHeight,
            halfWidth, -halfHeight,
            halfWidth, halfHeight
        };

        if(!Polygon::init(vertices, NUM_VERTICES, false)) {
            return false;
        }

    } else {
        float vertices[] = {
            0.0f, 0.0f, 0.5f, 0.5f,
            halfWidth, halfHeight, 1.0f, 1.0f,
            -halfWidth, halfHeight, 0.0f, 1.0f,
            -halfWidth, -halfHeight, 0.0f, 0.0f,
            halfWidth, -halfHeight, 1.0f, 0.0f,
            halfWidth, halfHeight, 1.0f, 1.0f
        };

        if(!Polygon::init(vertices, NUM_VERTICES, true)) {
            return false;
        }
    }

    m_width = width0;
    m_height = height0;

    return true;
}

} // end of namespace bot
