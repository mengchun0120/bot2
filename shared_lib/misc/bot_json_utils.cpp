#include <cstdio>
#include <list>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstring>
#include <rapidjson/filereadstream.h>
#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"

namespace bot {

struct JsonNode {
    JsonNode()
        : m_val(nullptr)
        , m_level(0)
    {}

    JsonNode(const JsonNode &other);

    bool init(const rapidjson::Value *val, unsigned int level,
              unsigned int indent);

    bool isStart();

    bool reachEnd();

    char brac();

    char ket();

    bool empty();

    bool forward();

    const rapidjson::Value &currentValue();

    const rapidjson::Value *m_val;
    union {
        rapidjson::Value::ConstValueIterator m_arrItr;
        rapidjson::Value::ConstMemberIterator m_memItr;
    };
    unsigned m_level;
    std::string m_indent;
};

JsonNode::JsonNode(const JsonNode &other)
{
    m_val = other.m_val;
    m_level = other.m_level;
    m_indent = other.m_indent;

    if (other.m_val->IsObject())
    {
        m_memItr = other.m_memItr;
    }
    else
    {
        m_arrItr = other.m_arrItr;
    }
}

bool JsonNode::init(const rapidjson::Value *val, unsigned int level,
                    unsigned int indent)
{
    if (!val)
    {
        LOG_ERROR("val is null");
        return false;
    }

    if (val->IsObject())
    {
        m_memItr = val->MemberBegin();
    }
    else if (val->IsArray())
    {
        m_arrItr = val->Begin();
    }
    else
    {
        LOG_ERROR("Val must be object or array");
        return false;
    }

    m_val = val;
    m_level = level;

    unsigned int numSpaces = level * indent;
    if (numSpaces > 0)
    {
        m_indent = std::string(numSpaces, ' ');
    }

    return true;
}

bool JsonNode::isStart()
{
    return m_val->IsObject() ? m_memItr == m_val->MemberBegin() :
                               m_arrItr == m_val->Begin();
}

bool JsonNode::reachEnd()
{
    return m_val->IsObject() ? m_memItr == m_val->MemberEnd() :
                               m_arrItr == m_val->End();
}

char JsonNode::brac()
{
    return m_val->IsObject() ? '{' : '[';
}

char JsonNode::ket()
{
    return m_val->IsObject() ? '}' : ']';
}

bool JsonNode::empty()
{
    return m_val->IsObject() ? m_val->ObjectEmpty() : m_val->Empty();
}

bool JsonNode::forward()
{
    if (m_val->IsObject())
    {
        if (m_memItr == m_val->MemberEnd())
        {
            return false;
        }

        m_memItr++;
    }
    else
    {
        if (m_arrItr == m_val->End())
        {
            return false;
        }

        m_arrItr++;
    }

    return true;
}

const rapidjson::Value &JsonNode::currentValue()
{
    if (m_val->IsObject())
    {
        return m_memItr->value;
    }

    return *m_arrItr;
}


bool readJson(rapidjson::Document &doc, const char *fileName)
{
    FILE *fp = fopen(fileName, "rb");
    if (!fp)
    {
        LOG_ERROR("Cannot open %s", fileName);
        return false;
    }

    char readBuffer[1000];

    rapidjson::FileReadStream stream(fp, readBuffer, sizeof(readBuffer));
    doc.ParseStream(stream);

    fclose(fp);

    if (doc.HasParseError())
    {
        LOG_ERROR("Failed to parse %s", fileName);
        return false;
    }

    return true;
}
void writeJsonValue(std::ofstream &os, const rapidjson::Value &val)
{
    if (val.IsNull())
    {
        os << "null";
    }
    else if (val.IsInt())
    {
        os << val.GetInt();
    }
    else if (val.IsString())
    {
        os << '"' << val.GetString() << '"';
    }
    else if (val.IsDouble())
    {
        os << std::fixed << std::setprecision(6) << val.GetDouble();
    }
    else if (val.IsBool())
    {
        os << (val.GetBool() ? "true" : "false");
    }
}

bool writeJson(const rapidjson::Document &doc, const char *fileName,
               unsigned int indent)
{
    std::list<JsonNode> stack;
    std::ofstream os(fileName);
    std::string singleIndent(indent, ' ');

    if (!os.good())
    {
        LOG_ERROR("Failed to open file %S", fileName);
        return false;
    }

    JsonNode node;

    if (!node.init(&doc, 0, indent))
    {
        LOG_ERROR("Failed to init node");
        return false;
    }

    if (node.empty())
    {
        os << node.brac() << node.ket();
        return true;
    }

    stack.push_back(node);
    os << node.brac();

    while (!stack.empty())
    {
        JsonNode &n = stack.back();

        if (n.reachEnd())
        {
            os << '\n' << n.m_indent << n.ket();
            stack.pop_back();
            continue;
        }

        if (!n.isStart())
        {
            os << ",";
        }

        os << '\n' << n.m_indent << singleIndent;

        if (n.m_val->IsObject())
        {
            os << '"' << n.m_memItr->name.GetString() << "\": ";
        }

        const rapidjson::Value &val = n.currentValue();

        if (val.IsObject() || val.IsArray())
        {
            JsonNode node1;
            if (!node1.init(&val, n.m_level + 1, indent))
            {
                LOG_ERROR("Failed to init node");
                return false;
            }


            if (node1.empty())
            {
                os << node1.brac() << node1.ket();
            }
            else
            {
                os << node1.brac();
                stack.push_back(node1);
            }
        }
        else
        {
            writeJsonValue(os, val);
        }

        n.forward();
    }

    os.close();

    return true;
}


bool parseJson(std::vector<JsonParamPtr> &params,
               const rapidjson::Value &elem)
{
    for (auto it = params.begin(); it != params.end(); ++it)
    {
        JsonParamPtr &p = *it;
        if (!p->parse(elem))
        {
            LOG_ERROR("Failed to parse %s", p->getName().c_str());
            return false;
        }
    }

    return true;
}

} // end of namespace bot

