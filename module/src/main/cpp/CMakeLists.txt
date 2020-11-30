cmake_minimum_required(VERSION 3.4.1)

if (NOT DEFINED MODULE_NAME)
    message(FATAL_ERROR "MODULE_NAME is not set")
endif ()

add_definitions(-DRIRU_MODULE)
add_definitions(-DRIRU_MODULE_API_VERSION=${RIRU_MODULE_API_VERSION})
add_definitions(-DRIRU_MODULE_VERSION=${RIRU_MODULE_VERSION})
add_definitions(-DRIRU_MODULE_VERSION_NAME=${RIRU_MODULE_VERSION_NAME})

message("Build type: ${CMAKE_BUILD_TYPE}")

set(DobbyHome Dobby)
include_directories(
        ${DobbyHome}/include
        ${DobbyHome}/external/logging
)

add_library( # Sets the name of the library.
        ${MODULE_NAME}
        # Sets the library as a shared library.
        SHARED
        # Provides a relative path to your source file(s).
        main.cpp
        hook_main.cpp)

macro(SET_OPTION option value)
    set(${option} ${value} CACHE INTERNAL "" FORCE)
endmacro()
#SET_OPTION(DOBBY_DEBUG ON)
SET_OPTION(DOBBY_GENERATE_SHARED OFF)
add_subdirectory(${DobbyHome} dobby)
target_link_libraries(${MODULE_NAME} log dobby)
