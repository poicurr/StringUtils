cmake_minimum_required(VERSION 3.20)
project(StringUtilsTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0
)
FetchContent_MakeAvailable(Catch2)

add_executable(tests test/test.cpp)
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

