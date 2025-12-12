cmake_minimum_required(VERSION 3.16)
project(HFTBacktester CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable warnings
if (MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB SOURCES CONFIGURE_DEPENDS src/*.cpp)
# Build core library without main.cpp for tests
set(CORE_SOURCES
  src/data_loader.cpp
  src/backtester.cpp
  src/advanced_backtester.cpp
)
add_library(hft_core ${CORE_SOURCES})
target_include_directories(hft_core PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_executable(hft_backtester src/main.cpp)
target_link_libraries(hft_backtester PRIVATE hft_core)

add_executable(hft_advanced src/main_advanced.cpp)
target_link_libraries(hft_advanced PRIVATE hft_core)

# Tests
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS tests/*.cpp)
add_executable(hft_tests ${TEST_SOURCES})
target_link_libraries(hft_tests PRIVATE hft_core)

