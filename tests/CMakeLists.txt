# -------- GTest --------

if(WIN32)
    set(GTEST_ROOT ${DEVTEST_LIBSDIR}/win32/gtest)
endif()
find_package(GTest REQUIRED)

add_executable(
    unit_tests
    main.cpp
    components/testFaceCV.cpp
)

target_link_libraries(
    unit_tests
    faceCV
    ${GTEST_LIBRARIES}
)

target_include_directories(
    unit_tests
    PRIVATE ${GTEST_INCLUDE_DIRS}
)

add_test(
  NAME
    unit
  COMMAND
    ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/unit_tests
  )