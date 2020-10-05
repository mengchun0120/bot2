#ifndef INCLUDE_BOT_MATH_UTILS
#define INCLUDE_BOT_MATH_UTILS

#include "misc/bot_constants.h"

namespace bot {

template <typename T>
T clamp(const T& t, const T& min, const T& max)
{
    if (t < min)
    {
        return min;
    }
    else if (t > max)
    {
        return max;
    }

    return t;
}

template <typename T>
inline bool between(const T& t, const T& a, const T& b)
{
    return (t >= a && t <= b) || (t >= b && t <= a);
}

inline float safeDivide(float x, float y)
{
    return (y > Constants::FLOAT_ZERO || y < -Constants::FLOAT_ZERO) ?
                                        (x / y) : (x / Constants::FLOAT_ZERO);
}

void fillColor(float* color, int r, int g, int b, int alpha);

void rotate(float& x, float& y, float directionX, float directionY);

void calculateDirection(float& directionX, float& directionY,
                        float srcX, float srcY, float dstX, float dstY);

float dist(float x1, float y1, float x2, float y2);

int compare(float a, float b, float threshold);

bool validateDirection(float directionX, float directionY);

} // end of namespace bot

#endif
