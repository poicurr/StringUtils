cmake_minimum_required(VERSION 3.50)
project(StringUtilsTest)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0
)
FetchContent_MakeAvailable(Catch2)

include_directories(include)

add_executable(tests test/test.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

