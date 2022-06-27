cmake_minimum_required(VERSION 3.22.2)

project(RayTracing_app)
set(CMAKE_CXX_STANDARD 17)

add_executable(RayTracingApp
	src/RayTracerApp.cpp
)

target_link_libraries(RayTracingApp core)