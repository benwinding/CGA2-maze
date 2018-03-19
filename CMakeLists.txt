cmake_minimum_required(VERSION 3.10)
project(assign2)

set(CMAKE_CXX_STANDARD 11)

add_executable(assign2 main.cpp shader.cpp engine.cpp)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(glfw3 3.2 REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS})

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
endif()

target_link_libraries(assign2
        glfw
        ${OPENGL_gl_LIBRARY}
        ${GLUT_LIBRARY}
        ${GLEW_LIBRARY})

configure_file(cube.frag cube.frag COPYONLY)
configure_file(cube.vert cube.vert COPYONLY)