cmake_minimum_required(VERSION 2.8.3 FATAL_ERROR)

project(spring_structure)

find_package(OpenCV REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(VTK REQUIRED)

include(${VTK_USE_FILE})

include_directories(${OPENGL_INCLUDE_DIRS}
                    ${GLUT_INCLUDE_DIRS}
                    /Users/danielpihlquist/dev_libraries/DevIL/DevIL/include/)


set(IL_LIB_DIR /Users/danielpihlquist/dev_libraries/DevIL/DevIL/build/lib/x64/)

set(WORK_DIR ${CMAKE_CURRENT_LIST_DIR}/)

set (CMAKE_SKIP_RULE_DEPENDENCY TRUE)
set (CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")

add_executable (spring_structure spring_structure_main.cpp 
                ${WORK_DIR}colors.cpp ${WORK_DIR}colors.h
                ${WORK_DIR}functions.cpp ${WORK_DIR}functions.h
                ${WORK_DIR}SpringStructure.cpp ${WORK_DIR}SpringStructure.h
                ${WORK_DIR}opengl_stuff.cpp ${WORK_DIR}opengl_stuff.h)

target_link_libraries (spring_structure
                       ${OpenCV_LIBS}
                       ${OPENGL_LIBRARIES}
                       ${GLUT_LIBRARY}
                       ${OpenCV_LIBS}
                       ${VTK_LIBRARIES}
                       ${IL_LIB_DIR}libIL.dylib
                       ${IL_LIB_DIR}libILU.dylib
                       ${IL_LIB_DIR}libILUT.dylib)