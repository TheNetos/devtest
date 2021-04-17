cmake_minimum_required(VERSION 3.0)

project(faceCV VERSION 1.0)

# ------- OpenCV --------

set(OpenCV_STATIC ON) # find static libs
set(OpenCV_DIR ${DEVTEST_LIBSDIR}/win32/opencv)
find_package(OpenCV REQUIRED)

# -----------------------

add_library(faceCV SHARED faceCV.cpp)
set(faceCV_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(faceCV PROPERTIES 
    PUBLIC_HEADER ${faceCV_INCLUDE_DIRS}/faceCV.h)
target_include_directories(faceCV PUBLIC ${OpenCV_INCLUDE_DIRS} ${faceCV_INCLUDE_DIRS}/..)
target_link_libraries(faceCV PUBLIC ${OpenCV_LIBS})
install(TARGETS faceCV
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
