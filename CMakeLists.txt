cmake_minimum_required(VERSION 3.15.7)
project(log C)

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release")
endif() # CMAKE_BUILD_TYPE

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
	set(LOG_IS_TOP_LVL ON)
else()
	set(LOG_IS_TOP_LVL OFF)
endif() # LOG_IS_TOP_LVL

if (LOG_IS_TOP_LVL)
	set(LOG_DEFAULT_LIB_TYPE "EXEC")
else()
	set(LOG_DEFAULT_LIB_TYPE "STATIC")
endif() # LOG_DEFAULT_LIB_TYPE

set(LOG_LIB_TYPE "${LOG_DEFAULT_LIB_TYPE}" CACHE STRING "Library type.")

if (LOG_LIB_TYPE STREQUAL "SHARED")
	add_library(${PROJECT_NAME} SHARED)
	target_compile_definitions(${PROJECT_NAME} PUBLIC LOG_LIB_SHARED)
elseif (LOG_LIB_TYPE STREQUAL "EXEC")
	add_executable(${PROJECT_NAME}
		${CMAKE_CURRENT_SOURCE_DIR}/tests/test.c)
	target_compile_definitions(${PROJECT_NAME} PUBLIC LOG_LIB_EXEC)
else()
	add_library(${PROJECT_NAME} STATIC)
	target_compile_definitions(${PROJECT_NAME} PUBLIC LOG_LIB_STATIC)
endif() # LOG_LIB_TYPE

target_include_directories(${PROJECT_NAME} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include)

if (NOT WIN32)
	target_sources(${PROJECT_NAME} PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src/posix.c)
endif() # WIN32

if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
        target_compile_options(${PROJECT_NAME} PRIVATE
                -fmacro-prefix-map=${CMAKE_CURRENT_SOURCE_DIR}=)
endif() # Compilers
