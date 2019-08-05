#include <boost/foreach.hpp>
#include <iostream>
#include "JsonHandler.h"

#ifdef __RAPIDJSON__
#include "rapidjson/document.h"		    // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h"	    // for stringify JSON
#include "rapidjson/filereadstream.h"   // FileReadStream
#include "rapidjson/istreamwrapper.h"   // IStreamWrapper

using namespace rapidjson;
#endif

using namespace std;

void readJsonObject(Value& val, uint8_t tab);
void readJsonArray(Value& val, uint8_t tab);

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
    cout << "JsonHandler::readXml(" << filename << ")" << endl;
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
        ss << "  \"SearchResult\": {" << endl;
        int tab = 2;
        BOOST_FOREACH(pt::ptree::value_type& child, _tree.get_child("nrf-pdu"))
        {
        /*
            std::string tag;
            pt::ptree sub_pt;
            std::tie(tag, sub_pt) = child;

            NodeType pt_type = getTypeAttr(sub_pt);
            cout << setTab(1) <<  "tag: " << tag << ", type: " << pt_type << endl;
        */
            std::string tag = child.first;
            NodeType pt_type = getXmlAttrType(child.second);

            if (pt_type.type == JsonType::ARRAY || pt_type.type == JsonType::SEQUENCE)
            {
                ss << setTab(tab) <<  "\"" << child.first << "\": [" << endl;
                getXmlArray(child.second, pt_type.name, tab);
                ss << setTab(tab) << "]";
            }
            else if (pt_type.type == JsonType::STRING)
            {
                ss << setTab(tab) <<  "\"" << child.first << "\": \"" << child.second.data() << "\"";
            }
            else if (pt_type.type == JsonType::INTEGER)
            {
                ss << setTab(tab) <<  "\"" << child.first << "\": " << child.second.data();
            }

            if (idx != child_size-1) { ss << "," << endl; }
            else { ss << endl; }
            idx++;
        }
        ss << "  }" << endl;
        ss << "}" << endl;
    }
    catch (const std::exception & ex)
    {
        cerr << "[JsonHandler::readXml] ERROR: " << ex.what() << endl;
        return false;
    }
    return true;
}

void JsonHandler::getXmlArray(pt::ptree& pt, const std::string& tname, uint8_t tab)
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

        NodeType pt_type = getXmlAttrType(child.second);
        if (pt_type.type == JsonType::ARRAY)
        {
            ss << setTab(tab+1) <<  "\"" << child.first << "\": [" << endl;
            getXmlArray(child.second, pt_type.name, tab+1);
            ss << setTab(tab+1) << "]";
        }
        else if (pt_type.type == JsonType::SEQUENCE)
        {
            ss << setTab(tab+1) << "{" << endl;
            getXmlSequence(child.second, pt_type.name, tab+1);
            ss << setTab(tab+1) << "}";
        }
        else if (pt_type.type == JsonType::STRING)
        {
            ss << setTab(tab+1) << "\"" << child.first << "\": \"" << child.second.data();
        }
        else if (pt_type.type == JsonType::INTEGER)
        {
            ss << setTab(tab+1) << "\"" << child.first << "\": " << child.second.data();
        }

        if (idx != child_size-1) { ss << "," << endl; }
        else { ss << endl; }
        idx++;
    }
}

void JsonHandler::getXmlSequence(pt::ptree& pt, const std::string& tname, uint8_t tab)
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

        NodeType pt_type = getXmlAttrType(child.second);

        ss << setTab(tab+1) <<  "\"" << child.first << "\": ";
        if (pt_type.type == JsonType::ARRAY)
        {
            ss << "[" << endl;
            getXmlArray(child.second, pt_type.name, tab+1);
            ss << setTab(tab+1) << "]";
        }
        else if (pt_type.type == JsonType::SEQUENCE)
        {
            ss << "{" << endl;
            getXmlSequence(child.second, pt_type.name, tab+1);
            ss << setTab(tab+1) << "}";
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

std::string getJsonValType(int ntype)
{
    static const char* kTypeNames[] = { "Null", "False", "True", "Object", "Array", "String", "Number" }; 
    std::stringstream ss;
    ss << kTypeNames[ntype] << "(" << ntype << ")";
    return ss.str();
}

void readJsonObject(const Value::Object& obj, uint8_t tab)
{
    for (Value::ConstMemberIterator itr = obj.MemberBegin(); itr != obj.MemberEnd(); ++itr)
    {
        std::string name = itr->name.GetString();
        int type = itr->value.GetType();
        cout << setTab(tab) << "\"" << name << "\" : ";
        switch (type)
        {
        case 4: // Array
            cout << getJsonValType(type) << endl;
            readJsonArray(obj[name.c_str()], tab+1);
            break;
        case 5: // String
            cout << getJsonValType(type) << " : \"" << itr->value.GetString() << "\"" << endl;
            break;
        case 6: // Number
            cout << getJsonValType(type) << " : " << itr->value.GetInt() << endl;
            break;
        }
    }
}

void readJsonArray(Value& val, uint8_t tab)
{
    assert(val.IsArray());
    assert(val.Size() != 0);
    cout << setTab(tab) << "array size : " << val.Size() << endl;
    int idx = 0;
    for (Value::ConstValueIterator itr = val.Begin(); itr != val.End(); ++itr)
    for (auto& v : val.GetArray())
    {
        cout << setTab(tab) << "[" << idx++ << "] : ";
        if (v.IsObject())
        {
            cout << getJsonValType(v.GetType()) << endl;
            readJsonObject(v.GetObject(), tab+1);
        }
        else if (v.IsArray())
        {
            cout << getJsonValType(v.GetType()) << endl;
        }
        else if (v.IsInt())
        {
            cout << "Number Parsing ..." << endl;
        }
        else if (v.IsString())
        {
            cout << "String Parsing ..." << endl;
        }
    }
}

void ParseNode(const rapidjson::Value &node, size_t indent=0, unsigned int level=0, const std::string& nodeName="");

std::string GetIndentString(size_t indent=0, unsigned int level=0)
{
    return std::move(std::string(level * indent, ' '));
}

void ParseObject(const rapidjson::Value &node, size_t indent = 0, unsigned int level = 0)
{
    std::cout << "{\n";

    for (rapidjson::Value::ConstMemberIterator childNode = node.MemberBegin(); childNode != node.MemberEnd(); ++childNode)
    {
        ParseNode(childNode->value, indent, level + 1, childNode->name.GetString());
    }

    std::cout << GetIndentString(indent, level) << "}";
}

void ParseArray(const rapidjson::Value& node, size_t indent = 0, unsigned int level = 0)
{
    std::cout << "[\n";

    for (rapidjson::SizeType i = 0; i < node.Size(); ++i)
    {
        ParseNode(node[i], indent, level + 1);
    }

    std::cout << GetIndentString(indent, level) << "]";
}

void ParseNode(const rapidjson::Value &node, size_t indent, unsigned int level, const std::string& nodeName)
{
    cout << GetIndentString(indent, level);
    if (!nodeName.empty())
        std::cout << nodeName << ": ";

    if (node.IsBool())
        std::cout << node.GetBool();

    else if (node.IsInt())
        std::cout << node.GetInt();

    else if (node.IsUint())
        std::cout << node.GetUint();

    else if (node.IsInt64())
        std::cout << node.GetInt64();

    else if (node.IsUint64())
        std::cout << node.GetUint64();

    else if (node.IsDouble())
        std::cout << node.GetDouble();

    else if (node.IsString())
        std::cout << node.GetString();

    else if (node.IsArray())
    {
        if (!nodeName.empty()) std::cout << "\n" << GetIndentString(indent, level);
            ParseArray(node, indent, level);
    }

    else if (node.IsObject())
    {
        if (!nodeName.empty()) std::cout << "\n" << GetIndentString(indent, level);
            ParseObject(node, indent, level);
    }

    std::cout << "\n";
}

bool JsonHandler::readJson(const std::string& filename)
{
    std::string filename1("02_nnrf-disc_nf-instances_get.json");
    cout << "JsonHandler::readJson(" << filename1 << ")" << endl;
    try
    {
#ifdef __RAPIDJSON__
        Document doc;
        
        ////////////////////////////////////////////////////////////////////////////
        // 1. Parse a JSON text string to a document.
        std::ifstream ifs("02_nnrf-disc_nf-instances_get.json"); 
        IStreamWrapper isw(ifs);
        if (doc.ParseStream(isw).HasParseError())
        {
            cerr << "[ERROR] HasParseError ..." << endl;
            return false;
        }
        cout << "Parsing to document succeeded.\n" << endl;

        ////////////////////////////////////////////////////////////////////////////
        // 2. Access values in document.
        setMarkerLine();
        cout << "# Access values in document:\n" << endl;
        assert(doc.IsObject());    // Document is a JSON value represents the root of DOM. Root can be either an object or array.

        assert(doc.HasMember("nfInstances"));
/*
        for (Value::ConstMemberIterator itr = doc.MemberBegin(); itr != doc.MemberEnd(); ++itr)
        {
            std::string name = itr->name.GetString();
            int type = itr->value.GetType();
            cout << "\"" << name << "\" : ";
            switch (type)
            {
            case 4: // Array
                cout << getJsonValType(type) << endl;
                readJsonArray(doc[name.c_str()], 1);
                break;
            }
        }
*/
        ParseNode(doc, 2);
#else // property_tree
        pt::ptree _pt;
        pt::read_json(filename, _pt);

        pt::ptree result = _pt.get_child("SearchResult");
        cout << "@ validityPeriod = " << result.get<int>("validityPeriod") << endl;

        cout << "child size = " << result.size() << endl;
        BOOST_FOREACH(pt::ptree::value_type& child, result)
        {
            if (child.first == "validityPeriod")
            {
                cout << "validityPeriod = " << child.second.data() << endl;
            }
            else if (child.first == "nrfSupportedFeatures")
            {
                cout << "nrfSupportedFeatures = " << child.second.data() << endl;
            }
            else
            {
                cout << child.first << endl;
                BOOST_FOREACH(pt::ptree::value_type& child1, child.second)
                {
                    cout << "  " << child1.first << endl;
                }
            }
        }
#endif
        return true;
    }
    catch (const std::exception & ex)
    {
        cerr << "[JsonHandler::readJson] ERROR: " << ex.what() << endl;
    }
    return false;
}

bool JsonHandler::writeJson(const std::string& filename)
{
    cout << "JsonHandler::writeJson(" << filename << ")" << endl;
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
            Document doc;
            const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";
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

NodeType JsonHandler::getXmlAttrType(pt::ptree& node)
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
