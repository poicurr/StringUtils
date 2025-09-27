# StringUtils

A simple and reusable collection of C++ header-only string utilities.  
Available via `#include <StringUtils/StringUtils.hpp>`.

## Features

- Header-only (no linking required)
- Easily integrated using `FetchContent_Declare` or `add_subdirectory`
- Unit tests via Catch2 (optionally enabled)

## Usage (CMake)

### Example using FetchContent

```cmake
include(FetchContent)
FetchContent_Declare(
  StringUtils
  GIT_REPOSITORY https://github.com/poicurr/StringUtils.git
  GIT_TAG main)

FetchContent_MakeAvailable(StringUtils)

target_link_libraries(MyApp PRIVATE StringUtils)
```

### Include Example

```cpp
#include <StringUtils/StringUtils.hpp>

int main() {
  std::string input = “  hello world  ”;
  std::string trimmed = strutil::trim(input);
  // trimmed == “hello world”
}
```

## Enabling Tests (Optional)

By default, tests are not built. To enable tests:

```cmake
set(STRINGUTILS_ENABLE_TESTS ON CACHE BOOL “” FORCE)
FetchContent_MakeAvailable(StringUtils)
```

Afterwards, you can run them via CTest or manually:

```sh
ctest    # Via CTest
./stringutils_test  # Manual execution
```

## License

MIT License

