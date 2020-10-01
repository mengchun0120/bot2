#ifndef INCLUDE_BOT_GAME_OBJECT
#define INCLUDE_BOT_GAME_OBJECT

#include "misc/bot_constants.h"
#include "structure/bot_double_linked_item.h"
#include "gametemplate/bot_game_object_template.h"

namespace bot {

class GameScreen;
class Graphics;

class GameObject: public DoubleLinkedItem {
public:
    GameObject();

    virtual ~GameObject()
    {}

    bool init(const GameObjectTemplate* t, float x, float y);

    GameObjectType getType() const
    {
        return m_template->getType();
    }

    virtual void present(Graphics& g) = 0;

    virtual void update(float delta, GameScreen& screen) = 0;

    float getPosX() const
    {
        return m_pos[0];
    }

    float getPosY() const
    {
        return m_pos[1];
    }

    void setPosX(float x)
    {
        m_pos[0] = x;
    }

    void setPosY(float y)
    {
        m_pos[1] = y;
    }

    float getCoverBreath() const
    {
        return m_template->getCoverBreath();
    }

    float getCollideBreath() const
    {
        return m_template->getCollideBreath();
    }

    float getCoverLeft() const
    {
        return m_pos[0] - m_template->getCoverBreath();
    }

    float getCoverBottom() const
    {
        return m_pos[1] - m_template->getCoverBreath();
    }

    float getCoverRight() const
    {
        return m_pos[0] + m_template->getCoverBreath();
    }

    float getCoverTop() const
    {
        return m_pos[1] + m_template->getCoverBreath();
    }

    float getCollideLeft() const
    {
        return m_pos[0] - m_template->getCollideBreath();
    }

    float getCollideBottom() const
    {
        return m_pos[1] - m_template->getCollideBreath();
    }

    float getCollideRight() const
    {
        return m_pos[0] + m_template->getCollideBreath();
    }

    float getCollideTop() const
    {
        return m_pos[1] + m_template->getCollideBreath();
    }

    int getCoverStartRow() const
    {
        return m_coverStartRow;
    }

    void setCoverStartRow(int startRow)
    {
        m_coverStartRow = startRow;
    }

    int getCoverEndRow() const
    {
        return m_coverEndRow;
    }

    void setCoverEndRow(int endRow)
    {
        m_coverEndRow = endRow;
    }

    int getCoverStartCol() const
    {
        return m_coverStartCol;
    }

    void setCoverStartCol(int startCol)
    {
        m_coverStartCol = startCol;
    }

    int getCoverEndCol() const
    {
        return m_coverEndCol;
    }

    void setCoverEndCol(int endCol)
    {
        m_coverEndCol = endCol;
    }

    void setCoverRect(int startRow, int endRow, int startCol, int endCol)
    {
        m_coverStartRow = startRow;
        m_coverEndRow = endRow;
        m_coverStartCol = startCol;
        m_coverEndCol = endCol;
    }

    void clearFlag(int flag)
    {
        m_flags &= ~flag;
    }

    void clearAllFlags()
    {
        m_flags = 0;
    }

    void setFlag(int flag)
    {
        m_flags |= flag;
    }

    bool testFlag(int flag) const
    {
        return (m_flags & flag) != 0;
    }

    int getFlags() const
    {
        return m_flags;
    }

protected:
    const GameObjectTemplate* m_template;
    int m_coverStartRow, m_coverEndRow;
    int m_coverStartCol, m_coverEndCol;
    int m_flags;
    float m_pos[Constants::NUM_FLOATS_PER_POSITION];
};

} // end of namespace bot

#endif
