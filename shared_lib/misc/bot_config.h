#ifndef INCLUDE_BOT_CONFIG
#define INCLUDE_BOT_CONFIG

#include <string>

namespace bot {

class Config {
public:
    Config();

    ~Config();

    bool load(const char* fileName);

public:
    int m_width;
    int m_height;
    std::string m_title;
    int m_timeDeltaHistoryLen;
    int m_eventQueueSize;
    std::string m_resourceFolder;
    std::string m_textureFolder;
    std::string m_animationFolder;
    std::string m_templateLibFolder;
};

} // end of namespace bot

#endif
