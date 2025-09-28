#pragma once

#include <cerrno>
#include <charconv>
#include <cstdlib>
#include <string>
#include <string_view>
#include <system_error>
#include <type_traits>

namespace strutil {
namespace detail {

template <class T> struct ParseResult {
  T value{};
  std::errc ec{};
  explicit operator bool() const noexcept { return ec == std::errc{}; }
};

template <class T, class Enable = void> struct ParseTraitsDefault;

template <class T>
struct ParseTraitsDefault<
    T, typename std::enable_if<std::is_integral<T>::value &&
                               !std::is_same<T, bool>::value>::type> {
  static ParseResult<T> parse(std::string_view s) {
    T ret{};
    auto *first = s.data();
    auto *last = s.data() + s.size();
    auto result = std::from_chars(first, last, ret);
    if (result.ec != std::errc{} || result.ptr != last)
      return {{}, result.ec != std::errc{} ? result.ec : std::errc::invalid_argument};
    return {ret, {}};
  }
};

template <class T>
struct ParseTraitsDefault<
    T, typename std::enable_if<std::is_floating_point<T>::value>::type> {
  static ParseResult<T> parse(std::string_view s) {
    std::string buffer(s);
    char *end = nullptr;
    errno = 0;
    long double work = std::strtold(buffer.c_str(), &end);
    if (end != buffer.c_str() + buffer.size() || errno)
      return {{}, std::errc::invalid_argument};
    return {static_cast<T>(work), {}};
  }
};

template <> struct ParseTraitsDefault<std::string, void> {
  static ParseResult<std::string> parse(std::string_view s) {
    return {std::string(s), {}};
  }
};

template <> struct ParseTraitsDefault<bool, void> {
  static ParseResult<bool> parse(std::string_view s) {
    if (s.empty())
      return {false, {}};
    if (s == "1" || s == "true" || s == "on" || s == "yes")
      return {true, {}};
    if (s == "0" || s == "false" || s == "off" || s == "no")
      return {false, {}};
    return {{}, std::errc::invalid_argument};
  }
};

} // namespace detail
} // namespace strutil
