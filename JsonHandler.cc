#include <boost/foreach.hpp>
#include <iostream>
#include "JsonHandler.h"

#ifdef __RAPIDJSON__
#include "rapidjson/document.h"		// rapidjson's DOM-style API
#include "rapidjson/prettywriter.h"	// for stringify JSON

using namespace rapidjson;
#endif

using namespace std;

NodeType getTypeAttr(pt::ptree& node);

void setMarkerLine()
{
    cout << "--------------------------------------------------" << endl;
}

void JsonHandler::initMap()
{
    try
    {
        pt::ptree cfg;
        pt::read_json("config.json", cfg);

        BOOST_FOREACH(auto& vt, cfg.get_child("array"))
        {
            assert(vt.first.empty());
            auto data = vt.second.data();
            arrayTypeList.push_back(data);
        }

        cout << "# array type list" << endl;
        for (auto it = arrayTypeList.begin(); it != arrayTypeList.end(); it++)
        {
            cout << *it << endl;
        }

        setMarkerLine();
    }
    catch (const std::exception & ex)
    {
        cerr << "[JsonHandler::initMap] ERROR: " << ex.what() << endl;
    }
}

bool JsonHandler::findType(const std::string& name)
{
    auto it = std::find(arrayTypeList.begin(), arrayTypeList.end(), name);
    if (it != arrayTypeList.end())
    {
        cout << "'" << name << "' found" << endl;
    }
    else
    {
        cout << "'" << name << "' not found" << endl;
    }
    return true;
}

bool JsonHandler::readXml(const std::string& filename)
{
    try
    {
        // Create empty property tree object
        //pt::ptree pt;

        // Parse the XML into the property tree
        pt::read_xml(filename, _tree);

        // the size of root will be 1
        cout << "root size : " << _tree.size() << endl;

        // 1st child size
        size_t child_size = _tree.get_child("nrf-pdu").size();
        cout << "# child size of 'nrf-pdu' : " << child_size << endl;

        int idx = 0;

        ss << "{" << endl;
        BOOST_FOREACH(pt::ptree::value_type& child, _tree.get_child("nrf-pdu"))
        {
        /*
            std::string tag;
            pt::ptree sub_pt;
            std::tie(tag, sub_pt) = child;

            NodeType pt_type = getTypeAttr(sub_pt);
            cout << getTab(1) <<  "tag: " << tag << ", type: " << pt_type << endl;
        */
            std::string tag = child.first;
            NodeType pt_type = getTypeAttr(child.second);

            if (pt_type.type == JsonType::ARRAY || pt_type.type == JsonType::SEQUENCE)
            {
                ss << getTab(1) <<  "\"" << child.first << "\": [" << endl;
                getXmlArray(child.second, pt_type.name, 1);
                ss << getTab(1) << "]";
            }
            else if (pt_type.type == JsonType::STRING)
            {
                ss << getTab(1) <<  "\"" << child.first << "\": \"" << child.second.data() << "\"";
            }
            else if (pt_type.type == JsonType::INTEGER)
            {
                ss << getTab(1) <<  "\"" << child.first << "\": " << child.second.data();
            }

            if (idx != child_size-1) { ss << "," << endl; }
            else { ss << endl; }
            idx++;
        }
        ss << "}" << endl;
        ss << endl;
    }
    catch (const std::exception & ex)
    {
        cerr << "[JsonHandler::readXml] ERROR: " << ex.what() << endl;
        return false;
    }
    return true;
}

void JsonHandler::getXmlArray(pt::ptree& pt, const std::string& tname, const int tab)
{
    size_t child_size = pt.size();
    //cout << "(getXmlArray) child size of '" << tname << "' : " << child_size << endl;

    int idx = 0;
    BOOST_FOREACH(pt::ptree::value_type& child, pt)
    {
        std::string tag = child.first;

        // <xmlattr> will be discarded ...
        if (tag == "<xmlattr>")
        {
            //cout << "(getXmlArray) <xmlattr>skipped ..." << endl;
            idx++;
            continue;
        }

        //assert(tname == tag);

        NodeType pt_type = getTypeAttr(child.second);
        if (pt_type.type == JsonType::ARRAY)
        {
            ss << getTab(tab+1) <<  "\"" << child.first << "\": [" << endl;
            getXmlArray(child.second, pt_type.name, tab+1);
            ss << getTab(tab+1) << "]";
        }
        else if (pt_type.type == JsonType::SEQUENCE)
        {
            ss << getTab(tab+1) << "{" << endl;
            getXmlSequence(child.second, pt_type.name, tab+1);
            ss << getTab(tab+1) << "}";
        }
        else if (pt_type.type == JsonType::STRING)
        {
            ss << getTab(tab+1) << "\"" << child.first << "\": \"" << child.second.data();
        }
        else if (pt_type.type == JsonType::INTEGER)
        {
            ss << getTab(tab+1) << "\"" << child.first << "\": " << child.second.data();
        }

        if (idx != child_size-1) { ss << "," << endl; }
        else { ss << endl; }
        idx++;
    }
}

void JsonHandler::getXmlSequence(pt::ptree& pt, const std::string& tname, const int tab)
{
    size_t child_size = pt.size();
    //cout << "(getXmlSequence) child size of '" << tname << "' : " << child_size << endl;

    int idx = 0;
    BOOST_FOREACH(pt::ptree::value_type& child, pt)
    {
        std::string tag = child.first;

        // <xmlattr> will be discarded ...
        if (tag == "<xmlattr>")
        {
            //cout << "(getXmlSequence) <xmlattr>skipped ..." << endl;
            idx++;
            continue;
        }

        NodeType pt_type = getTypeAttr(child.second);

        ss << getTab(tab+1) <<  "\"" << child.first << "\": ";
        if (pt_type.type == JsonType::ARRAY)
        {
            ss << "[" << endl;
            getXmlArray(child.second, pt_type.name, tab+1);
            ss << getTab(tab+1) << "]";
        }
        else if (pt_type.type == JsonType::SEQUENCE)
        {
            ss << "{" << endl;
            getXmlSequence(child.second, pt_type.name, tab+1);
            ss << getTab(tab+1) << "}";
        }
        else if (pt_type.type == JsonType::STRING)
        {
            ss << "\"" << child.second.data() << "\"";
        }
        else if (pt_type.type == JsonType::INTEGER)
        {
            ss << child.second.data();
        }

        if (idx != child_size-1) { ss << "," << endl; }
        else { ss << endl; }
        idx++;
    }
}

bool JsonHandler::writeJson(const std::string& filename)
{
    try
    {
        ofstream json_file(filename);
        //cout << ss.str() << endl;
        std::string data = ss.str(); 
        json_file << data;
        json_file.close();

#ifdef __JSONCPP__
        Json::Value root;

        ofstream json_file;
        json_file.open(filename);

        Json::StreamWriterBuilder builder;
        builder["commentStyle"] = "None";
        builder["indentation"] = "  ";  // tab, or whatever you like

        std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
        writer->write(root, &cout);
        cout << endl; // add lf and flush

        writer->write(root, &json_file);
        json_file.close();
#endif

#ifdef __RAPIDJSON__
        cout << "--------------------------------------------------" << endl;
        cout << "[ RapidJSON #1 ]" << endl;
        cout << "--------------------------------------------------" << endl;
        {
            // 1. Parse a JSON string into DOM
            const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";
            Document doc;
            doc.Parse(json);

            // 2. Modify it by DOM
            Value& s = doc["stars"];
            s.SetInt(s.GetInt() + 1);

            // 3. Stringify the DOM
            StringBuffer buffer;
            PrettyWriter<StringBuffer> writer(buffer);
            writer.SetIndent(' ', 2);
            doc.Accept(writer);

            cout << buffer.GetString() << endl;
        }

        cout << "--------------------------------------------------" << endl;
        cout << "[ RapidJSON #2 ]" << endl;
        cout << "--------------------------------------------------" << endl;
        {
            StringBuffer buffer;
            PrettyWriter<StringBuffer> writer(buffer);
            writer.SetIndent(' ', 2);
            writer.StartObject();
            writer.Key("hello");
            writer.String("world");
            writer.Key("t");
            writer.Bool(true);
            writer.Key("f");
            writer.Bool(false);
            writer.Key("n");
            writer.Null();
            writer.Key("i");
            writer.Uint(123);
            writer.Key("pi");
            writer.Double(3.1416);
            writer.Key("a");
            writer.StartArray();
            for (unsigned i = 0; i < 4; i++)
                writer.Uint(i);
            writer.EndArray();
            writer.EndObject();
            cout << buffer.GetString() << endl;
        }
#endif
    } 
    catch (const std::exception & ex)
    {
        cerr << "[JsonHandler::writeJson] ERROR: " << ex.what() << endl;
        return false;
    }
    return true;
}

NodeType JsonHandler::getTypeAttr(pt::ptree& node)
{
    try
    {
        std::string type = node.get<std::string>("<xmlattr>.type");

        std::size_t found = type.find("array");
        if (found != std::string::npos)
        {
            NodeType node_type = { JsonType::ARRAY, type.substr(6) };
            return node_type;
        }

        found = type.find("string");
        if (found != std::string::npos)
        {
            return STRING_TYPE;
        }

        found = type.find("integer");
        if (found != std::string::npos)
        {
            return INTEGER_TYPE;
        }
    }
    catch (const std::exception & ex)
    {
        //cerr << "[isArrayType] ERROR: " << ex.what() << endl;
    }
    return SEQUENCE_TYPE;
}
