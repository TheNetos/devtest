cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 11)

project(devtest VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(DEVTEST_LIBSDIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)

# ------ Boost lib ------
set(Boost_DEBUG ON)
set(Boost_ARCHITECTURE      -x32)
set(Boost_USE_STATIC_RUNTIME ON)
set(Boost_USE_STATIC_LIBS    ON)
set(Boost_USE_DEBUG_LIBS     OFF)
set(Boost_USE_RELEASE_LIBS   ON)
set(Boost_USE_MULTITHREADED  ON)

if(WIN32)
  # Needed for boost to turn off autolinking for dynamic linking
  add_definitions(-DBOOST_ALL_NO_LIB)
endif()

# add_definitions(-DBOOST_LOG_DYN_LINK)

find_package(Boost 1.70 REQUIRED COMPONENTS system container program_options thread log)
include_directories(${Boost_INCLUDE_DIRS})
if (NOT Boost_FOUND)
    message(FATAL_ERROR "Boost library not found in '${BOOST_ROOT}'")    
endif()

# -----------------------

add_subdirectory(source/components)
add_executable(devtest source/main.cpp source/core/AppCore.hpp source/core/AppCore.cpp)

target_link_libraries(devtest PRIVATE faceCV PUBLIC ${Boost_LIBRARIES})

enable_testing()
add_subdirectory(tests)

# Copy face recognition cascade file to result build dir
if(NOT EXISTS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/haarcascade_frontalface_alt2.xml) 
    file(COPY source/haarcascade_frontalface_alt2.xml DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

# Copy test assets to result build dir
if(NOT EXISTS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/testassets) 
    file(COPY testassets DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()
