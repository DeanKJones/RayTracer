#========== INCLUDE ==========#

include(./.env.cmake OPTIONAL RESULT_VARIABLE LOCAL_ENV)
message(STATUS "Local .env.cmake: ${LOCAL_ENV}")

#========== BEGIN CMAKE ==========#

cmake_minimum_required(VERSION 3.22.2)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(RayTracerApplication)

#add_subdirectory(core)
add_subdirectory(external/glm)
#add_subdirectory(src)

#========== CHECK COMPILER ==========#

message(STATUS "using ${CMAKE_GENERATOR}")
if (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
    if (NOT MINGW_PATH) 
      message(FATAL_ERROR "MINGW_PATH not set in .env.cmake")
    endif()
    set(USE_MINGW "True")
    set(CMAKE_C_COMPILER ${MINGW_PATH}/bin/gcc.exe)
    set(CMAKE_CXX_COMPILER  ${MINGW_PATH}/bin/g++.exe)
endif()

#========== CREATE EXECUTABLE ==========#

add_executable(${PROJECT_NAME} src/RayTracerApp.cpp)

#========== CHECK SUBMODULES ==========#

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

#========== FIND PACKAGES ==========#

#find_package(glfw3 REQUIRED)
#find_package(glm REQUIRED)

    #========== GLFW ==========#

set(GLFW_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw)
set(GLFW_INCLUDES ${GLFW_DIR}/include)

   #========== VULKAN ==========#

# Find VULKAN package
if (DEFINED VULKAN_SDK_PATH)                                                # Defined path should be saved in a .env.cmake file
    message(STATUS "Found user defined Vulkan_SDK_Path")
    set(Vulkan_INCLUDE_DIRS "${VULKAN_SDK_PATH}/macOS/include")             # 1.1 Make sure this include path is correct
    set(Vulkan_LIBRARIES "${VULKAN_SDK_PATH}/macOS/lib")                    # 1.2 Make sure lib path is correct
    set(Vulkan_FOUND "True")
else()
    find_package(Vulkan REQUIRED)
    message(STATUS "Found Vulkan: $ENV{VULKAN_SDK}")
endif()
# Check and Write to console
if (NOT VULKAN_FOUND)
	message(FATAL_ERROR "Could not find Vulkan library!")
else()
    message(STATUS "Using vulkan lib at: ${Vulkan_LIBRARIES}")
endif()

#set(VULKAN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/vulkan)
#message(STATUS "Got the Vulkan folder here: ${VULKAN_DIR}")  

#============== IMGUI ==============#

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui)
message(STATUS "Got the IMGUI folder here: ${IMGUI_DIR}")
add_library(imgui 
        STATIC
            ${IMGUI_DIR}/imgui_demo.cpp
            ${IMGUI_DIR}/imgui_draw.cpp
            ${IMGUI_DIR}/imgui_tables.cpp
            ${IMGUI_DIR}/imgui_widgets.cpp
            
            ${IMGUI_DIR}/imconfig.h
            ${IMGUI_DIR}/imgui_internal.h

            ${IMGUI_DIR}/backends/imgui_impl_glfw.h
            ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
            ${IMGUI_DIR}/backends/imgui_impl_vulkan.h
            ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp

            ${IMGUI_DIR}/imgui.h
            ${IMGUI_DIR}/imgui.cpp
        )

#========== OS ==========#

if (APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    SET(EXTRA_LIBS ${COCOA_LIBRARY} ${OpenGL_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY})
endif (APPLE)

#========== Link ==========#

include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${IMGUI_DIR}
    ${GLFW_INCLUDES}
    ${VULKAN_DIR}/include    
)

target_link_libraries(${PROJECT_NAME}
    imgui
    ${Vulkan_LIBRARIES}
    ${EXTRA_LIBS}
)
target_link_directories(${PROJECT_NAME}
PUBLIC
    imgui
    imgui/backends
)