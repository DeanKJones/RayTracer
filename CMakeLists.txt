cmake_minimum_required(VERSION 3.22.2)
project(RayTracerApplication)

add_subdirectory(core)
add_subdirectory(external)
add_subdirectory(src)

message(STATUS "using ${CMAKE_GENERATOR}")
if (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
    if (NOT MINGW_PATH) 
      message(FATAL_ERROR "MINGW_PATH not set in .env.cmake")
    endif()
    set(USE_MINGW "True")
    set(CMAKE_C_COMPILER ${MINGW_PATH}/bin/gcc.exe)
    set(CMAKE_CXX_COMPILER  ${MINGW_PATH}/bin/g++.exe)
endif()

add_executable(${PROJECT_NAME} src/RayTracerApp.cpp)

# Download all the submodules 
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    # Update submodules
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}")
        endif()
    endif()
endif()
# Check all the submodules
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/external/glfw/CMakeLists.txt")
    message(FATAL_ERROR "The glfw submodules were not downloaded! GIT_SUBMODULES was turned off or failed.")
endif()       


# Find VULKAN package
find_package(Vulkan REQUIRED)

# Found
message(STATUS "Found Vulkan: ${VULKAN_FOUND}")
set(Vulkan_FOUND "True")
# Check and Write to console
if (NOT Vulkan_FOUND)
	message(FATAL_ERROR "Could not find Vulkan library!")
else()
    message(STATUS "Using vulkan lib at: ${Vulkan_LIBRARIES}")
    target_link_libraries(${PROJECT_NAME} ${Vulkan_LIBRARIES})
    include_directories(${Vulkan_INCLUDE_DIRS})
endif()

#============== GLFW ==============#

set(GLFW_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw)
add_subdirectory(${GLFW_DIR})

#============== IMGUI ==============#

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui)
add_library(IMGUI STATIC)

target_sources(IMGUI
    PRIVATE
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui.cpp
    PRIVATE
    ${IMGUI_DIR}/backends/imgui_impl_glfw.h
    ${IMGUI_DIR}/backends/imgui_impl_vulkan.h
)
target_include_directories(IMGUI
    PUBLIC ${IMGUI_DIR}
    PUBLIC ${IMGUI_DIR}/backends
)

#set(GLM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/glm)

find_path(STB_IMAGE_INCLUDE_DIRS "stb_image.h")

# target_link_directories(${PROJECT_NAME}
#     PRIVATE
#     glfw
#     glm::glm
#     imgui::imgui
# )