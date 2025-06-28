cmake_minimum_required(VERSION 3.20)
project(StringUtilsTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(StringUtils INTERFACE)
target_include_directories(StringUtils INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

option(STRINGUTILS_ENABLE_TESTS "Enable building tests for StringUtils" OFF)

if(STRINGUTILS_ENABLE_TESTS)
  enable_testing()

  include(FetchContent)
  FetchContent_Declare(
	catch2
	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
	GIT_TAG        v3.4.0
  )
  FetchContent_MakeAvailable(Catch2)

  add_executable(stringutils_test test/test.cpp)
  target_include_directories(stringutils_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
  target_link_libraries(stringutils_test PRIVATE Catch2::Catch2WithMain)

  add_test(NAME StringUtilsTests COMMAND stringutils_test)
endif()
