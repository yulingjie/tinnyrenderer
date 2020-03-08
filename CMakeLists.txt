cmake_minimum_required(VERSION 3.16)

project(Renderer)

include_directories("${PROJECT_SOURCE_DIR}/deps")
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

find_package(glfw3)

set(GLAD_GL "${PROJECT_SOURCE_DIR}/deps/glad/gl.h"
            "${PROJECT_SOURCE_DIR}/deps/glad_gl.c")
set(TGA "${PROJECT_SOURCE_DIR}/deps/tga/tgaimage.h"
		"${PROJECT_SOURCE_DIR}/deps/tga/tgaimage.cpp")
set(MODEL "${PROJECT_SOURCE_DIR}/deps/model.h"
		"${PROJECT_SOURCE_DIR}/deps/model.cpp"
		"${PROJECT_SOURCE_DIR}/deps/geometry.h"
		"${PROJECT_SOURCE_DIR}/deps/geometry.cpp"
		)
set (MATH "${PROJECT_SOURCE_DIR}/deps/linmath.h")
set(OBJ "${PROJECT_SOURCE_DIR}/obj/african_head.obj")
add_executable(line 
	./src/main.cpp
	./src/shaders.h
	./src/shaders.cpp
	${MATH}
	${GLAD_GL}
	${TGA}
	${MODEL})
message(STATUS ${GLFW3_INCLUDE_DIR})
message(STATUS ${GLFW3_LIBRARY})

target_link_libraries(line 
${GLFW3_LIBRARY}
)
message(STATUS ${CMAKE_BUILD_TYPE})

target_include_directories(line PUBLIC
${GLFW3_INCLUDE_DIR})

if(NOT DEFINED CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug")
endif()
file(COPY ${PROJECT_SOURCE_DIR}/obj DESTINATION "${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/")
file(COPY ${PROJECT_SOURCE_DIR}/obj DESTINATION "${PROJECT_BINARY_DIR}/")
