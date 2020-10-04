#ifndef INCLUDE_BOT_MOVER_TEMPLATE
#define INCLUDE_BOT_MOVER_TEMPLATE

#include <string>
#include "gametemplate/bot_single_unit_template.h"

namespace bot {

class MoverTemplate: public SingleUnitTemplate {
public:
    class Parser {
    public:
        Parser(const NamedMap<Texture>& textureLib,
               const NamedMap<Rectangle>& rectLib)
            : m_textureLib(textureLib)
            , m_rectLib(rectLib)
        {}

        ~Parser()
        {}

        MoverTemplate* create(const std::string& name,
                              const rapidjson::Value& elem);

    private:
        const NamedMap<Texture>& m_textureLib;
        const NamedMap<Rectangle>& m_rectLib;
    };

public:
    MoverTemplate();

    virtual ~MoverTemplate()
    {}

    bool init(const NamedMap<Texture>& textureLib,
              const NamedMap<Rectangle>& rectLib,
              const rapidjson::Value& elem);

    float getSpeed(int level) const;

    bool setSpeed(float speed);

    bool setSpeedPerLevel(float speedPerLevel);

protected:
    float m_speed;
    float m_speedPerLevel;
};

} // end of namespace bot

#endif

