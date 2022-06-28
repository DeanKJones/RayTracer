if not exist build mkdir build
cd build
cmake -DGLFW_BUILD_DOCS=OFF -S ../ -B . -G "MinGW Makefiles"
mingw32-make.exe
cd ..