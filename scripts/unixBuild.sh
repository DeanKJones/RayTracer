#!/bin/sh 

mkdir -p build
cd ./build
cmake -DGLFW_BUILD_DOCS=OFF -S ../ -B .
make -j1
cd ..