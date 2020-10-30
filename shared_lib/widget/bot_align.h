#ifndef INCLUDE_BOT_ALIGN
#define INCLUDE_BOT_ALIGN

namespace bot {

enum Align {
    ALIGN_HLEFT,
    ALIGN_HMIDDILE,
    ALIGN_HRIGHT,
    ALIGN_VTOP,
    ALIGN_VMIDDLE,
    ALIGN_VBOTTOM
};

inline bool isValidHAlign(Align align)
{
    return align >= ALIGN_HLEFT && align <= ALIGN_HRIGHT;
}

inline bool isValidVAlign(Align align)
{
    return align >= ALIGN_VTOP && align <= ALIGN_VBOTTOM;
}

} // end of namespace bot

#endif

