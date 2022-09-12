#!/bin/sh 

mkdir -p build
cd ./build
cmake -DCMAKE_BUILD_TYPE=Debug -DGLFW_BUILD_DOCS=OFF -S ../ -B .
make -j1
cd ..