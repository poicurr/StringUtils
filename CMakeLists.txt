cmake_minimum_required(VERSION 3.20)
project(StringUtils LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(StringUtils INTERFACE)
target_include_directories(StringUtils INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# Install support toggled by option that also works when embedded
set(STRINGUTILS_IS_TOP_LEVEL OFF)
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
  set(STRINGUTILS_IS_TOP_LEVEL ON)
endif()

set(_stringutils_default_install OFF)
if(STRINGUTILS_IS_TOP_LEVEL)
  set(_stringutils_default_install ON)
endif()
option(STRINGUTILS_ENABLE_INSTALL "Enable install/export targets for StringUtils" ${_stringutils_default_install})
unset(_stringutils_default_install)

set(STRINGUTILS_EXPORT_SET "StringUtilsTargets" CACHE STRING "Export set used when installing StringUtils")
set(STRINGUTILS_INSTALL_EXPORT_FILE ON CACHE BOOL "Install the StringUtils export file")
set(STRINGUTILS_EXPORT_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/StringUtils" CACHE PATH "Destination for the StringUtils export file")

if(STRINGUTILS_ENABLE_INSTALL)
  include(GNUInstallDirs)

  install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/StringUtils)
  install(TARGETS StringUtils EXPORT ${STRINGUTILS_EXPORT_SET})

  if(STRINGUTILS_INSTALL_EXPORT_FILE)
    install(EXPORT ${STRINGUTILS_EXPORT_SET} NAMESPACE StringUtils:: DESTINATION ${STRINGUTILS_EXPORT_DESTINATION})
  endif()
endif()

# Optional Catch2-based tests
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
  target_include_directories(stringutils_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
  target_link_libraries(stringutils_test PRIVATE Catch2::Catch2WithMain)

  add_test(NAME StringUtilsTests COMMAND stringutils_test)
endif()

