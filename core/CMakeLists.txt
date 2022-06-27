cmake_minimum_required(VERSION 3.22.2)
project(RayTracerLib)

# ========= setup ================
set(CMAKE_CXX_STANDARD 17)

# for cmake modules if u wnat to add some libraries
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")

# flags for gcc or clang
set(GCC_COVERAGE_COMPILE_FLAGS "")

if(NOT MSVC)
	set(CMAKE_CXX_FLAGS	"${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
endif()

# ===============================

# ========= deps ================
# vulkan loader
find_package(Vulkan REQUIRED)

set(VULKAN_DIR ${PROJECT_SOURCE_DIR}/external/vulkan)
message(STATUS "Got the Vulkan folder here: ${VULKAN_DIR}") 

# Include Vulkan
include_directories(${VULKAN_DIR}/include)


# glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_VULKAN_STATIC ON CACHE BOOL "" FORCE)
add_subdirectory(external/glfw)

# glm
set(BUILD_TESTING OFF)
add_subdirectory(external/glm)

# imgui
add_subdirectory(external/imgui)

#stb-image
add_subdirectory(external/stb_image)
# ===============================

# ========== Core =============
# find all source files and headers
file(
	GLOB_RECURSE # recursive
	APP_SRC # variable to store the source files and headers
	CONFIGURE_DEPENDS # make a dependency
	"${PROJECT_SOURCE_DIR}/src/*.h" "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_library(core STATIC ${APP_SRC})
# =================================

# ========= linking deps ==========
target_link_libraries(core PUBLIC Vulkan::Vulkan)
target_link_libraries(core PUBLIC glfw)
target_link_libraries(core PUBLIC imgui)
target_link_libraries(core PUBLIC glm)
target_link_libraries(core PUBLIC stbimage)
# =================================

# ========== include dir ==========
target_include_directories(core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(core PUBLIC "${PROJECT_SOURCE_DIR}/vendor/stb_image")
# =================================