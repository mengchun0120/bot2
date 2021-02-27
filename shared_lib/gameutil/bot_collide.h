#ifndef INCLUDE_BOT_COLLIDE
#define INCLUDE_BOT_COLLIDE

namespace bot {

bool checkTouchBoundary(float &newDelta, float mapWidth, float mapHeight, float x, float y,
                        float collideBreathX, float collideBreathY, float speedX, float speedY, float delta);

bool checkObjCollision(float &newDelta, float x1, float y1, float collideBreathX1, float collideBreathY1,
                       float speedX, float speedY, float x2, float y2, float collideBreathX2, float collideBreathY2,
                       float delta);

bool checkRectOverlapp(float left1, float bottom1, float right1, float top1,
                       float left2, float bottom2, float right2, float top2);

} // end of namespace bot

#endif