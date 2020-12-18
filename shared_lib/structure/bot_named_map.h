#ifndef INCLUDE_BOT_NAMED_MAP
#define INCLUDE_BOT_NAMED_MAP

#include <list>
#include <vector>
#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"

namespace bot {

template <typename T>
class NamedMap {
public:
    NamedMap();

    virtual ~NamedMap();

    bool load(const char* fileName);

    bool load(const std::string& fileName)
    {
        return load(fileName.c_str());
    }

    template <typename P>
    bool load(const char* fileName, P& parser);

    template <typename P>
    bool load(const std::string& fileName, P& parser)
    {
        return load(fileName.c_str(), parser);
    }

    bool add(const char *name, T* t);

    bool add(const std::string &name, T* t)
    {
        return add(name.c_str(), t);
    }

    T* search(const char *name) const;

    T* search(const std::string &name) const
    {
        return search(name.c_str());
    }

    int getNumObjs() const
    {
        return static_cast<int>(m_objs.size());
    }

    T* getObjAt(int idx)
    {
        return m_objs[idx];
    }

    const T* getObjAt(int idx) const
    {
        return m_objs[idx];
    }

    const std::string& getNameAt(int idx) const
    {
        return m_names[idx];
    }

    void clear();

    void log() const;

private:
    struct Node {
        Node *m_left, *m_right;
        char m_ch;
        T* m_ptr;

        Node(char ch)
        : m_left(nullptr)
        , m_right(nullptr)
        , m_ch(ch)
        , m_ptr(nullptr)
        {}
    };

    Node *m_root;
    std::vector<T*> m_objs;
    std::vector<std::string> m_names;
};

template <typename T>
NamedMap<T>::NamedMap()
    : m_root(nullptr)
{}

template <typename T>
NamedMap<T>::~NamedMap()
{
    clear();
}

template <typename T>
bool NamedMap<T>::load(const char* fileName)
{
    rapidjson::Document doc;

    if (!readJson(doc, fileName))
    {
        return false;
    }

    if (!doc.IsArray())
    {
        LOG_ERROR("Invalid format: %s", fileName);
        return false;
    }

    const rapidjson::Value& arr = doc.GetArray();
    int numObjects = arr.Capacity();

    m_objs.resize(numObjects);
    m_names.resize(numObjects);
    for (int i = 0; i < numObjects; ++i)
    {
        const rapidjson::Value& elem = arr[i];
        std::string name;

        if (!JsonParser::parse(name, elem, "name"))
        {
            LOG_ERROR("Failed to find name in the %dth object of %s",
                      i, fileName);
            return false;
        }

        T* t = new T();
        if (!t->init(elem))
        {
            LOG_ERROR("Failed to parse the %dth object of %s", i, fileName);
            delete t;
            return false;
        }

        add(name, t);
        m_objs[i] = t;
        m_names[i] = name;
    }

    return true;
}

template <typename T>
template <typename P>
bool NamedMap<T>::load(const char* fileName, P& parser)
{
    rapidjson::Document doc;

    if (!readJson(doc, fileName))
    {
        return false;
    }

    if (!doc.IsArray())
    {
        LOG_ERROR("Invalid format: %s", fileName);
        return false;
    }

    const rapidjson::Value& arr = doc.GetArray();
    int numObjects = arr.Capacity();

    m_objs.resize(numObjects);
    m_names.resize(numObjects);
    for (int i = 0; i < numObjects; ++i)
    {
        const rapidjson::Value& elem = arr[i];
        std::string name;

        if (!JsonParser::parse(name, elem, "name"))
        {
            LOG_ERROR("Failed to find name in the %dth object of %s",
                      i, fileName);
            return false;
        }

        T* t = parser(name, elem);
        if (!t)
        {
            LOG_ERROR("Failed to parse the %dth object of %s", i, fileName);
            return false;
        }

        add(name, t);
        m_objs[i] = t;
        m_names[i] = name;
    }

    return true;
}


template <typename T>
bool NamedMap<T>::add(const char* name, T* t)
{
    const char* ch;
    Node* parent = nullptr;
    Node* n = m_root;

    for (ch = name; *ch != '\0' && n; ++ch)
    {
        Node* prev = nullptr;
        Node* m = n;

        while (m && m->m_ch < *ch)
        {
            prev = m;
            m = m->m_right;
        }

        if (m && m->m_ch == *ch)
        {
            parent = m;
            n = m->m_left;
        }
        else
        {
            Node* p = new Node(*ch);

            p->m_right = m;
            if (prev)
            {
                prev->m_right = p;
            }
            else if (parent)
            {
                parent->m_left = p;
            }
            else
            {
                m_root = p;
            }

            parent = p;
            n = nullptr;
        }
    }

    if (n)
    {
        if (n->m_ptr != t)
        {
            // a name can only map to one pointer
            LOG_WARN("Trying to add a different pointer for name %s", name);
            return false;
        }
        else
        {
            // alreay in the map
            return true;
        }
    }

    for (; *ch != '\0'; ++ch)
    {
        n = new Node(*ch);
        if (parent)
        {
            parent->m_left = n;
        }
        else
        {
            m_root = n;
        }
        parent = n;
    }

    if (parent) {
        parent->m_ptr = t;
    }

    return true;
}

template <typename T>
T* NamedMap<T>::search(const char* name) const
{
    Node* n = m_root;
    const char* ch = name;

    while (*ch != '\0' && n)
    {
        while (n && n->m_ch < *ch)
        {
            n = n->m_right;
        }

        if (!n || n->m_ch != *ch)
        {
            return nullptr;
        }

        ++ch;
        if (*ch != '\0')
        {
            n = n->m_left;
        }
    }

    return n ? n->m_ptr : nullptr;
}

template <typename T>
void NamedMap<T>::clear()
{
    if (!m_root)
    {
        return;
    }

    std::list<Node*> q;
    q.push_back(m_root);

    while (!q.empty())
    {
        Node* n = q.front();
        q.pop_front();

        if (n->m_left)
        {
            q.push_back(n->m_left);
        }

        if (n->m_right)
        {
            q.push_back(n->m_right);
        }

        if (n->m_ptr)
        {
            delete n->m_ptr;
        }

        delete n;
    }

    m_objs.clear();
    m_names.clear();
}

template <typename T>
void NamedMap<T>::log() const
{
    if (!m_root)
    {
        return;
    }

    std::list<Node*> q;
    q.push_back(m_root);

    while (!q.empty())
    {
        Node* n = q.front();
        q.pop_front();

        LOG_INFO("%p %c %p %p", n, n->m_ch, n->m_left, n->m_right);

        if (n->m_left)
        {
            q.push_back(n->m_left);
        }

        if (n->m_right)
        {
            q.push_back(n->m_right);
        }
    }
}

} // end of namespace bot

#endif
