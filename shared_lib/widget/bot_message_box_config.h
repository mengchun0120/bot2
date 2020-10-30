#ifndef INCLUDE_BOT_MESSAGE_BOX_CONFIG
#define INCLUDE_BOT_MESSAGE_BOX_CONFIG

#include <vector>
#include <string>
#include "misc/bot_constants.h"

namespace bot {

template <typename T> class NamedMap;
class Color;

class MessageBoxConfig {
public:
    MessageBoxConfig();

    ~MessageBoxConfig()
    {}

    bool init(const std::string& configFile,
              const NamedMap<Color>& colorLib);

    const Color* getBoxFillColor() const
    {
        return m_boxFillColor;
    }

    const Color* getBoxBorderColor() const
    {
        return m_boxBorderColor;
    }

    const Color* getTextColor() const
    {
        return m_textColor;
    }

private:
    const Color* m_boxFillColor;
    const Color* m_boxBorderColor;
    const Color* m_textColor;
};

} // end of namespace bot

#endif
