#!/bin/sh
#git pull origin master
cd Protocol
rm -rf ProtoFiles
git clone git@172.16.0.2:Nico/ProtoFiles.git
cd ProtoFiles
protoc -I=./ --cpp_out=../ *.proto
cd ../../build 
cmake ..
