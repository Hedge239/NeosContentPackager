cmake_minimum_required(VERSION 3.10.0)
project(NeoContentPackager VERSION 0.0.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")

# BUILD OPTIONS #
option(NCP_BUILD_TESTS "Builds Testing Applications" ON)
option(NCP_BUILD_UTILITY "Builds NCP CMD utility" ON)

option(NCP_INCLUDE_READER "Includes the ability to read .ncp files" ON)
    if(NCP_INCLUDE_READER) 
        include("${CMAKE_SOURCE_DIR}/cmake/.cmake/lib/reader.cmake")
    endif()

option(NCP_INCLUDE_PACKER "Includes the ability to create .ncp files" ON)
    if(NCP_INCLUDE_PACKER) 
        include("${CMAKE_SOURCE_DIR}/cmake/.cmake/lib/packer.cmake")
    endif()

add_library(${PROJECT_NAME} ${NCP_LIBCORE} "include/NeoContentPackager")
target_include_directories(${PROJECT_NAME}  PRIVATE "include/NeoContentPackager")

if(NCP_BUILD_TESTS)
    add_subdirectory(tests)
endif()

if(NCP_BUILD_UTILITY)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "NeoContentPackager" VERSION ${PROJECT_VERSION} SOVERSION "0" FOLDER "NeoContentPackager")