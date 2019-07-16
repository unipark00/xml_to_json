/*
 * Title : Define.h
 */

#ifndef DEFINE_H_
#define DEFINE_H_

enum class JsonType {
    ARRAY = 0, 
    SEQUENCE,
    STRING,
    INTEGER
};

typedef struct NodeType {
    JsonType        type;
    std::string     name;
} NodeType;

const NodeType ARRAY_TYPE    = { JsonType::ARRAY,    "" };
const NodeType SEQUENCE_TYPE = { JsonType::SEQUENCE, "" };
const NodeType STRING_TYPE   = { JsonType::STRING,   "" };
const NodeType INTEGER_TYPE  = { JsonType::INTEGER,  "" };

inline std::ostream& operator<<(std::ostream& os, const JsonType& type)
{
    switch (type) {
    case JsonType::ARRAY:
        os << "array";
        break;
    case JsonType::STRING:
        os << "string";
        break;
    case JsonType::INTEGER:
        os << "integer";
        break;
    case JsonType::SEQUENCE:
    default:
        os << "sequence";
        break;
    }
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const NodeType& type)
{
    switch (type.type) {
    case JsonType::ARRAY:
        os << "array(" << type.name << ")";
        break;
    case JsonType::STRING:
        os << "string";
        break;
    case JsonType::INTEGER:
        os << "integer";
        break;
    case JsonType::SEQUENCE:
    default:
        os << "sequence";
        break;
    }
    return os;
}

inline std::string getTab(const int tab)
{
    std::string tabstr = "";
    for (int i = 0; i < tab; i++) 
    {
        tabstr += "  ";
    }
    return tabstr;
}

#endif /* DEFINE_H_ */
