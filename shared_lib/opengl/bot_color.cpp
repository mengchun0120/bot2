#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "opengl/bot_color.h"

namespace bot {

Color* Color::Parser::create(const std::string& name,
                             const rapidjson::Value& elem)
{
    Color* color = new Color();
    if (!color->init(elem))
    {
        delete color;
        return nullptr;
    }
    return color;
}

bool Color::validateColor(int red, int green, int blue, int alpha)
{
    if (!validateElem(red))
    {
        LOG_ERROR("Invalid red value %d", red);
        return false;
    }

    if (!validateElem(green))
    {
        LOG_ERROR("Invalid green value %d", green);
        return false;
    }

    if (!validateElem(blue))
    {
        LOG_ERROR("Invalid blue value %d", blue);
        return false;
    }

    if (!validateElem(alpha))
    {
        LOG_ERROR("Invalid alpha value %d", alpha);
        return false;
    }

    return true;
}

Color::Color()
{
    m_color[0] = 0.0f;
    m_color[1] = 0.0f;
    m_color[2] = 0.0f;
    m_color[3] = 0.0f;
}

bool Color::init(const rapidjson::Value& elem)
{
    int red = 0, green = 0, blue = 0, alpha = 0;
    std::vector<JsonParamPtr> params =
    {
        jsonParam(red, "red"),
        jsonParam(green, "green"),
        jsonParam(blue, "blue"),
        jsonParam(alpha, "alpha")
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    if (!init(red, green, blue, alpha))
    {
        LOG_ERROR("Failed to set color");
        return false;
    }

    return true;
}

bool Color::init(int red, int green, int blue, int alpha)
{
    if (!validateColor(red, green, blue, alpha))
    {
        return false;
    }

    m_color[0] = red / 255.0f;
    m_color[1] = green / 255.0f;
    m_color[2] = blue / 255.0f;
    m_color[3] = alpha / 255.0f;

    return true;
}

bool Color::setRed(int red)
{
    if (!validateElem(red))
    {
        LOG_ERROR("Invalid red value %d", red);
        return false;
    }

    m_color[0] = red / 255.0f;

    return true;
}

bool Color::setRed(float red)
{
    if (!validateElem(red))
    {
        LOG_ERROR("Invalid red value %f", red);
        return false;
    }

    m_color[0] = red;

    return true;
}

bool Color::setGreen(int green)
{
    if (!validateElem(green))
    {
        LOG_ERROR("Invalid green value %d", green);
        return false;
    }

    m_color[1] = green / 255.0f;

    return true;
}

bool Color::setGreen(float green)
{
    if (!validateElem(green))
    {
        LOG_ERROR("Invalid green value %f", green);
        return false;
    }

    m_color[1] = green;

    return true;
}

bool Color::setBlue(int blue)
{
    if (!validateElem(blue))
    {
        LOG_ERROR("Invalid blue value %d", blue);
        return false;
    }

    m_color[2] = blue / 255.0f;

    return true;
}

bool Color::setBlue(float blue)
{
    if (!validateElem(blue))
    {
        LOG_ERROR("Invalid blue value %f", blue);
        return false;
    }

    m_color[2] = blue;

    return true;
}

bool Color::setAlpha(int alpha)
{
    if (!validateElem(alpha))
    {
        LOG_ERROR("Invalid alpha value %d", alpha);
        return false;
    }

    m_color[3] = alpha / 255.0f;

    return true;
}

bool Color::setAlpha(float alpha)
{
    if (!validateElem(alpha))
    {
        LOG_ERROR("Invalid alpha value %f", alpha);
        return false;
    }

    m_color[3] = alpha;

    return true;
}

} // end of namespace bot
