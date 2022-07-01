cmake_minimum_required(VERSION 3.22.2)

project(RayTracing_app)
set(CMAKE_CXX_STANDARD 17)

# ========== SRC =============
# find all source files and headers
file(
	GLOB_RECURSE 		# recursive
	SRC 		 		# variable to store the source files and headers
	CONFIGURE_DEPENDS 	# make a dependency
	"${PROJECT_SOURCE_DIR}/src/*.h" "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable(RayTracingApp ${SRC})
# =================================

target_link_libraries(RayTracingApp core)