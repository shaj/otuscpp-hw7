cmake_minimum_required(VERSION 3.2)


set(PATCH_VERSION 1)
if($ENV{TRAVIS_BUILD_NUMBER})
  set(PATCH_VERSION $ENV{TRAVIS_BUILD_NUMBER})
endif()


project(bulk VERSION 0.1.${PATCH_VERSION})

set(Boost_USE_STATIC_LIBS   ON)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)
find_package(Threads)

add_executable(bulk main.cpp bulk.cpp)

set_target_properties(bulk PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
  COMPILE_OPTIONS -Wpedantic -Wall -Wextra
  INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

target_link_libraries(bulk
  ${Boost_LIBRARIES}          # program_options
  ${CMAKE_THREAD_LIBS_INIT}   # spdlog
)

if((NOT CMAKE_BUILD_TYPE) OR (CMAKE_BUILD_TYPE EQUAL "Debug"))
  target_compile_definitions(bulk PRIVATE SPDLOG_TRACE_ON)
endif()

install(TARGETS bulk RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT kolobrod2002@yandex.ru)

include (CPack)

configure_file("${PROJECT_SOURCE_DIR}/version.h.in" "${PROJECT_BINARY_DIR}/version.h" @ONLY)
target_include_directories(bulk PRIVATE "${PROJECT_BINARY_DIR}")
target_include_directories(bulk PRIVATE spdlog/include)

configure_file("${PROJECT_SOURCE_DIR}/bintray.json.in" "${PROJECT_BINARY_DIR}/bintray.json")

configure_file("${PROJECT_SOURCE_DIR}/tests/t1.txt" "${PROJECT_BINARY_DIR}/t1.txt" COPYONLY)
configure_file("${PROJECT_SOURCE_DIR}/tests/t2.txt" "${PROJECT_BINARY_DIR}/t2.txt" COPYONLY)
configure_file("${PROJECT_SOURCE_DIR}/tests/t3.txt" "${PROJECT_BINARY_DIR}/t3.txt" COPYONLY)
configure_file("${PROJECT_SOURCE_DIR}/tests/t4.txt" "${PROJECT_BINARY_DIR}/t4.txt" COPYONLY)

enable_testing()

add_subdirectory(tests)
