#ifndef INCLUDE_BOT_PLAYER_TEMPLATE
#define INCLUDE_BOT_PLAYER_TEMPLATE

#include <string>
#include "gametemplate/bot_robot_template.h"

namespace bot {

class PlayerTemplate: public RobotTemplate {
public:
    PlayerTemplate();

    virtual ~PlayerTemplate();

    bool init(const std::string& playerTemplateFile, const NamedMap<ComponentTemplate>& componentLib);
};

} // end of namespace bot

#endif
