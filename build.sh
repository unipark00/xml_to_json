#!/bin/bash

#echo "g++ -o XmlToJson *.cc -D__RAPIDJSON__"
#g++ -o XmlToJson *.cc -D__RAPIDJSON__
rm XmlToJson
echo "g++ -o XmlToJson *.cc"
g++ -o XmlToJson *.cc
