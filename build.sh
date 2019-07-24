#!/bin/bash

rm XmlToJson

echo "g++ -o XmlToJson *.cc -D__RAPIDJSON__"
g++ -o XmlToJson *.cc -D__RAPIDJSON__

#echo "g++ -o XmlToJson *.cc"
#++ -o XmlToJson *.cc
