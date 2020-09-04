#ifndef INCLUDE_BOT_GAME_STATE
#define INCLUDE_BOT_GAME_STATE

namespace bot {

enum GameState {
    GAME_STATE_INIT,
    GAME_STATE_RUNNING,
    GAME_STATE_PAUSED,
    GAME_STATE_END
};

} // end of namespace bot

#endif
