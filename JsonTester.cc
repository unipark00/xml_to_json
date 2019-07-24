#include <iostream>
#include <fstream>
#include "JsonHandler.h"

using namespace std;

bool readXml();

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        cout << "[Usage] ./JsonTester <in:xmlfile> <out:jsonfile>" << endl;
        return false; 
    }

    auto handler = new JsonHandler();
    if (!handler->readXml(argv[1]))
    {
        cout << "JsonHandler::readXml(" << argv[1] << ") failed" << endl;
        return false; 
    }

    if (!handler->writeJson(argv[2]))
    {
        cout << "JsonHandler::writeJson(" << argv[2] << ") failed" << endl;
        return false; 
    }

    if (!handler->readJson(argv[2]))
    {
        cout << "JsonHandler::readJson(" << argv[2] << ") failed" << endl;
        return false; 
    }

    cout << endl;
    cout << "JsonTester main End." << endl;
    return true;
}
