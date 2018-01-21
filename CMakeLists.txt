cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(spring_structure)

find_package( OpenCV REQUIRED )
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

set(WORK_DIR /Users/danielpihlquist/programming_projects/spring_structure/)

set (CMAKE_SKIP_RULE_DEPENDENCY TRUE)
set (CMAKE_CXX_STANDARD 11)

if ( CMAKE_COMPILER_IS_GNUCC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall")
endif()
if ( MSVC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4")
endif()

add_executable (spring_structure spring_structure_main.cpp 
                ${WORK_DIR}colors.cpp ${WORK_DIR}colors.h
                ${WORK_DIR}functions.cpp ${WORK_DIR}functions.h
                ${WORK_DIR}SpringStructure.cpp ${WORK_DIR}SpringStructure.h
                ${WORK_DIR}opengl_stuff.cpp ${WORK_DIR}opengl_stuff.h
                ${WORK_DIR}BouncyBall.cpp ${WORK_DIR}BouncyBall.h)

target_link_libraries (spring_structure ${OpenCV_LIBS} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${OpenCV_LIBS})