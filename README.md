# StringUtils

シンプルで再利用可能な C++ ヘッダオンリーの文字列ユーティリティ集です。  
`#include <StringUtils/StringUtils.hpp>` で使えます。

## 特徴

- ヘッダオンリー（リンク不要）
- `string_utils` 名前空間で安全に設計
- `FetchContent_Declare` や `add_subdirectory` で簡単に導入可能
- Catch2 によるユニットテスト（任意で有効化）

## 使い方（CMake）

### FetchContent での使用例

```cmake
include(FetchContent)
FetchContent_Declare(
  StringUtils
  GIT_REPOSITORY https://github.com/poicurr/StringUtils.git
  GIT_TAG main
)
FetchContent_MakeAvailable(StringUtils)

target_link_libraries(MyApp PRIVATE StringUtils)
```

### include 例

```cpp
#include <StringUtils/StringUtils.hpp>

int main() {
  std::string input = "  hello world  ";
  std::string trimmed = string_utils::trim(input);
  // trimmed == "hello world"
}
```

## テストの有効化（任意）

デフォルトではテストはビルドされません。テストを有効にするには：

```cmake
set(STRINGUTILS_ENABLE_TESTS ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(StringUtils)
```

その後、CTestや手動で実行可能です：

```sh
ctest    # CTest 経由
./stringutils_test  # 手動実行
```

## ライセンス

MIT License
