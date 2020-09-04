#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include "misc/bot_log.h"
#include "misc/bot_config.h"

namespace bot {

Config::Config()
    : m_width(0)
    , m_height(0)
    , m_title("Battle Bot")
    , m_timeDeltaHistoryLen(5)
    , m_eventQueueSize(20)
{
}

Config::~Config()
{
}

bool Config::load(const char* fileName)
{
    using namespace rapidjson;

    FILE* fp = fopen(fileName, "rb");
    if (!fp) {
        LOG_ERROR("Failed to open %s", fileName);
        return false;
    }

    char readBuffer[1000];
    FileReadStream stream(fp, readBuffer, sizeof(readBuffer));
    Document doc;
    doc.ParseStream(stream);
    fclose(fp);

    if (doc.HasParseError()) {
        LOG_ERROR("Failed to parse %s", fileName);
        return false;
    }

    if (!doc.HasMember("width")) {
        LOG_ERROR("Config doesn't contain width");
        return false;
    }
    else {
        const Value& v = doc["width"];
        if (!v.IsInt()) {
            LOG_ERROR("Invalid width");
            return false;
        }
        m_width = v.GetInt();
    }

    if (!doc.HasMember("height")) {
        LOG_ERROR("Config doesn't contain height");
        return false;
    }
    else  {
        const Value& v = doc["height"];
        if (!v.IsInt()) {
            LOG_ERROR("Invalid height");
            return false;
        }
        m_height = v.GetInt();
    }

    if (doc.HasMember("title")) {
        const Value& v = doc["title"];
        if (!v.IsString()) {
            LOG_ERROR("Invalid title");
            return false;
        }
        m_title = v.GetString();
    }

    if (doc.HasMember("timeDeltaHistoryLen")) {
        const Value& v = doc["timeDeltaHistoryLen"];
        if (!v.IsInt()) {
            LOG_ERROR("Invalid timeDeltaHistoryLen");
            return false;
        }
        m_timeDeltaHistoryLen = v.GetInt();
    }

    if (doc.HasMember("eventQueueSize")) {
        const Value& v = doc["eventQueueSize"];
        if (!v.IsInt()) {
            LOG_ERROR("Invalid eventQueueSize");
            return false;
        }
        m_eventQueueSize = v.GetInt();
    }

    return true;
}

} // end of namespace bot

