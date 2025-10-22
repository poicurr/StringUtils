cmake_minimum_required(VERSION 3.28)
project(StringUtils LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_SCAN_FOR_MODULES ON)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  if (APPLE)
    add_compile_options(-stdlib=libc++)
    add_link_options(-stdlib=libc++)
  elseif (WIN32)
    if (CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC" OR
        CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
      # clang-cl + MSVC STL
    else()
      # MSYS2/llvm-mingw
      add_compile_options(-stdlib=libc++)
      add_link_options(-stdlib=libc++ -lc++abi -fuse-ld=lld)
    endif()
  else()
    add_compile_options(-stdlib=libc++)
    add_link_options(-stdlib=libc++ -lc++abi)
  endif()
endif()

add_library(StringUtils)
add_library(StringUtils::StringUtils ALIAS StringUtils)

target_compile_features(StringUtils INTERFACE cxx_std_20)

target_sources(StringUtils
  PUBLIC
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/StringUtils.cppm
    ${CMAKE_CURRENT_SOURCE_DIR}/src/detail/ParseTraitsDefault.cppm
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

if(STRINGUTILS_ENABLE_INSTALL)
  include(GNUInstallDirs)

  install(TARGETS StringUtils
    EXPORT ${STRINGUTILS_EXPORT_SET}
    FILE_SET cxx_modules DESTINATION ${CMAKE_INSTALL_DATADIR}/StringUtils/modules
  )

  if(STRINGUTILS_INSTALL_EXPORT_FILE)
    install(EXPORT ${STRINGUTILS_EXPORT_SET}
      NAMESPACE StringUtils::
      DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/StringUtils)
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

  add_executable(stringutils_test tests/tests.cpp)
  target_link_libraries(stringutils_test PRIVATE Catch2::Catch2WithMain StringUtils::StringUtils)
  add_test(NAME StringUtilsTests COMMAND stringutils_test)
endif()
