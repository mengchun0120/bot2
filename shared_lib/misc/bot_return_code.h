#ifndef INLCUDE_BOT_RETURN_CODE
#define INLCUDE_BOT_RETURN_CODE

namespace bot {

enum ReturnCode {
    RET_CODE_OK,
    RET_CODE_OUT_OF_SIGHT,           // Object has moved out of sight
    RET_CODE_COLLIDE                 // Object has collided with other objects
};

} // end of namespace bot

#endif
