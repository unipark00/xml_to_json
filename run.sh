#!/bin/bash
INPUT=nf-instances-get.xml
OUTPUT=nf-instances-get.json

#echo "--------------------------------------------------"
#cat nf-instances-get.xml

echo "--------------------------------------------------"
#./XmlToJson nf-instances-get.xml nf-instances-get.json 2> err.log
./XmlToJson nf-instances-get.xml nf-instances-get.json

if [ -f "$OUTPUT" ]; then
    echo "--------------------------------------------------"
    #cat nf-instances-get.json
fi
