cmake_minimum_required(VERSION 3.5)

project (Line)


option(DEBUG_PROFILE "enable ggdb profile" ON)
if (DEBUG_PROFILE)
set(GDB_PG_FLAG "-ggdb -g -pg -O3")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GDB_PG_FLAG}")
endif()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(TINY_INCLUDE_DIR ./tinyrenderer)

include_directories(${TINY_INCLUDE_DIR})

add_executable(Line main.cpp line.cpp triangle.cpp
    ./tinyrenderer/tgaimage.cpp
    ./tinyrenderer/model.cpp
    ./tinyrenderer/geometry.cpp
    ${TINY_INCLUDE_DIR}/tgaimage.h
    ${TINY_INCLUDE_DIR}/model.h
    line.h
    triangle.h
    )
#
#add_custom_command(TARGET Line 
#    POST_BUILD
#    COMMAND cp -rf $(CMAKE_SOURCE_DIR)/tinyrenderer/obj $(CMAKE_SOURCE_DIR)/build	
#	)
