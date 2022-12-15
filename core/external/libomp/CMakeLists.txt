#Directly add openMp to external libraries to avoid annoying Mac formalities

set(CMAKE_CXX_STANDARD 17)
CMAKE_MINIMUM_REQUIRED(VERSION 3.22.2)

add_library(libomp INTERFACE)

target_link_libraries(libomp INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/15.0.6/lib/libomp.dylib")
target_include_directories(libomp INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/15.0.6/include")