#ifndef INCLUDE_BOT_ACTION
#define INCLUDE_BOT_ACTION

namespace bot {

enum Action {
    ACTION_CHASE,
    ACTION_SHOOT,
    ACTION_NONE
};

inline bool isValidAction(Action a)
{
    return a >= ACTION_CHASE && a <= ACTION_NONE;
}

} // end of namespace bot

#endif
