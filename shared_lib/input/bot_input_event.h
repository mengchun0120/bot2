#ifndef INCLUDE_BOT_INPUT_EVENT
#define INCLUDE_BOT_INPUT_EVENT

namespace bot {

struct MouseButtonEvent {
    float m_x;
    float m_y;
    int m_button;
    int m_action;
};

struct MouseMoveEvent {
    float m_x;
    float m_y;
};

struct KeyEvent {
    int m_key;
    int m_action;
};

struct InputEvent {
    enum EventType {
        ET_MOUSE_BUTTON,
        ET_MOUSE_MOVE,
        ET_KEY
    };

    EventType m_type;
    union {
        MouseButtonEvent m_mouseButtonEvent;
        MouseMoveEvent m_mouseMoveEvent;
        KeyEvent m_keyEvent;
    };
};

} // end of namespace bot

#endif

