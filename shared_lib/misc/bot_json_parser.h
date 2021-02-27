#ifndef INCLUDE_BOT_JSON_PARSER
#define INCLUDE_BOT_JSON_PARSER

#include <string>
#include <vector>
#include <rapidjson/document.h>

namespace bot {

struct JsonParser {
    static bool parse(bool &r, const rapidjson::Value &elem,
                      const char *name, bool required=true);

    static bool parse(int &r, const rapidjson::Value &value,
                      const char *name, bool required=true);

    static bool parse(float &r, const rapidjson::Value &value,
                      const char *name, bool required=true);

    static bool parse(double &r, const rapidjson::Value &value,
                      const char *name, bool required=true);

    static bool parse(std::string &r, const rapidjson::Value &value,
                      const char *name, bool required=true);

    static bool parse(std::vector<int> &r, const rapidjson::Value &value,
                      const char *name, bool required=true);

    static bool parse(std::vector<float> &r, const rapidjson::Value &value,
                      const char *name, bool required=true);

    static bool parse(std::vector<double> &r, const rapidjson::Value &value,
                      const char *name, bool required=true);

    static bool parse(std::vector<bool> &r, const rapidjson::Value &value,
                      const char *name, bool required=true);

    static bool parse(std::vector<std::string> &r,
                      const rapidjson::Value &value,
                      const char *name, bool required=true);
};

} // end of namespace bot

#endif

