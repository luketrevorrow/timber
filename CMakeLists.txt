cmake_minimum_required(VERSION 3.0.0)
project(timber VERSION 0.1.0
               DESCRIPTION "Timberman clone"
               LANGUAGES CXX)

include(CTest)
enable_testing()
find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
add_executable(timber src/timber.cpp)
target_link_libraries(timber PRIVATE sfml-system sfml-network sfml-graphics sfml-window)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
