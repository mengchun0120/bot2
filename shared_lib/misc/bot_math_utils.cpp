#include <cmath>
#include "misc/bot_math_utils.h"

namespace bot {

void rotate(float& x, float& y, float directionX, float directionY)
{
    float x1 = x * directionX - y * directionY;
    float y1 = x * directionY + y * directionX;
    x = x1;
    y = y1;
}

void calculateDirection(float& directionX, float& directionY, float srcX, float srcY, float dstX, float dstY)
{
    float deltaX = dstX - srcX;
    float deltaY = dstY - srcY;
    float dist = sqrt(deltaX * deltaX + deltaY * deltaY);
    directionX = deltaX / dist;
    directionY = deltaY / dist;
}

float dist(float x1, float y1, float x2, float y2)
{
    float distX = x1 - x2;
    float distY = y1 - y2;
    return sqrt(distX * distX + distY * distY);
}

} // end of namespace bot
