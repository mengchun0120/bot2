#ifndef INCLUDE_BOT_GAME_OBJECT_ITEM
#define INCLUDE_BOT_GAME_OBJECT_ITEM

namespace bot {

class GameObject;

class GameObjectItem {
public:
    static bool match(const GameObjectItem* i1, const GameObjectItem* i2)
    {
        return i1->m_obj == i2->m_obj;
    }

    GameObjectItem()
        : m_obj(nullptr)
        , m_next(nullptr)
    {}

    virtual ~GameObjectItem()
    {}

    GameObject* getObj()
    {
        return m_obj;
    }

    const GameObject* getObj() const
    {
        return m_obj;
    }

    void setObj(GameObject* obj)
    {
        m_obj = obj;
    }

    GameObjectItem* getNext()
    {
        return m_next;
    }

    void setNext(GameObjectItem* n)
    {
        m_next = n;
    }

protected:
    GameObject* m_obj;
    GameObjectItem* m_next;
};

} // end of namespace bot

#endif
