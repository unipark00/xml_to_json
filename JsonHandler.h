/*
 * Title : JsonHandler.h
 */

#ifndef JSON_HANDLER_H_
#define JSON_HANDLER_H_

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <list>
#include <sstream>
#include "Define.h"

namespace pt = boost::property_tree;

class JsonHandler
{
private:
    pt::ptree               _tree;
    std::list<std::string>  arrayTypeList;
    std::stringstream       ss;

public:
    //JsonHandler() { initMap(); }

    bool        readXml(const std::string& filename);

    bool        readJson(const std::string& filename);
    bool        writeJson(const std::string& filename);

private:
    void        initMap();
    bool        findType(const std::string& name);

    void        readXmlArray(pt::ptree& pt, const std::string& tname, const int tab);
    void        readXmlSequence(pt::ptree& pt, const std::string& tname, const int tab);
    NodeType    getTypeAttr(pt::ptree& node);
};

#endif /* JSON_HANDLER_H_ */
