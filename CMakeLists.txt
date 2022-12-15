include(./.env.cmake OPTIONAL RESULT_VARIABLE LOCAL_ENV)
message(STATUS "Local .env.cmake: ${LOCAL_ENV}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(GCC_COVERAGE_COMPILE_FLAGS)
SET(CMAKE_CXX_FLAGS  "${GCC_COVERAGE_COMPILE_FLAGS}")

if(UNIX)
	set(tools "/opt/homebrew/bin")
	set(CMAKE_C_COMPILER "${tools}/gcc-12")
	message(Cmake C compiler: ${CMAKE_C_COMPILER})
	set(CMAKE_CXX_COMPILER "${tools}/g++-12")
	message(Cmake CXX compiler: ${CMAKE_CXX_COMPILER})
endif(UNIX)

# Create Project
cmake_minimum_required(VERSION 3.22.2)
project(RayTracing)

add_subdirectory(core)
add_subdirectory(app)

# will create compile commands link in the root of the project
execute_process(
	COMMAND ${CMAKE_COMMAND} -E create_symlink
		${CMAKE_BINARY_DIR}/compile_commands.json
		${CMAKE_SOURCE_DIR}/compile_commands.json
)