#ifndef INCLUDE_BOT_COLOR
#define INCLUDE_BOT_COLOR

#include <string>
#include <vector>
#include <rapidjson/document.h>
#include "misc/bot_constants.h"

namespace bot {

class Color {
public:
    class Parser {
    public:
        Parser()
        {}

        ~Parser()
        {}

        Color* create(const std::string& name,
                      const rapidjson::Value& elem);
    };

    static bool validateElem(int elem)
    {
        return elem >= 0 && elem <= 255;
    }

    static bool validateElem(float elem)
    {
        return elem >= 0.0f && elem <= 1.0f;
    }

    static bool validateColor(int red, int green, int blue, int alpha);

    static bool validateColor(float red, float green, float blue, float alpha);

    static Color* create(const rapidjson::Value& elem);

    Color();

    Color(const Color& other);

    ~Color()
    {}

    bool init(const rapidjson::Value& elem);

    bool init(int red, int green, int blue, int alpha);

    bool init(float red, float green, float blue, float alpha);

    bool init(const std::vector<int> colorVec);

    bool init(const std::vector<float> colorVec);

    Color& operator=(const Color& other);

    Color& operator*=(const Color& other);

    const float *getColor() const
    {
        return m_color;
    }

    float getRed() const
    {
        return m_color[0];
    }

    bool setRed(int red);

    bool setRed(float red);

    float getGreen() const
    {
        return m_color[1];
    }

    bool setGreen(int green);

    bool setGreen(float green);

    float getBlue() const
    {
        return m_color[2];
    }

    bool setBlue(int blue);

    bool setBlue(float blue);

    float getAlpha() const
    {
        return m_color[3];
    }

    bool setAlpha(int alpha);

    bool setAlpha(float alpha);

private:
    float m_color[Constants::NUM_FLOATS_COLOR];
};

} // end of namespace bot

#endif
