cmake_minimum_required(VERSION 3.22.2)
project(RayTracerGeo)

set(CMAKE_CXX_STANDARD 17)

# ========== SRC =============
# find all source files and headers
file(
	GLOB_RECURSE 		# recursive
	SRC_GEO 		 	# variable to store the source files and headers
	CONFIGURE_DEPENDS 	# make a dependency
	"${PROJECT_SOURCE_DIR}/*.h" "${PROJECT_SOURCE_DIR}/*.cpp"
)

message(STATUS "Adding Source Code Geometry Files: ${SRC_GEO}")
add_executable(RayTracerGeo ${SRC_GEO})
# =================================

#target_link_libraries(RayTracerGeo core)