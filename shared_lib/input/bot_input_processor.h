#ifndef INCLUDE_BOT_INPUT_PROCESSOR
#define INCLUDE_BOT_INPUT_PROCESSOR

#include <functional>

namespace bot {

struct InputEvent;

// Prototype of input event processor
// The processor returns:
// 0, proceed to next input;
// 1, clear the event queue and stop processing
// 2, clear the event queue and exit app
typedef std::function<int(const InputEvent&)> InputProcessor;

} // end of namespace bot

#endif
