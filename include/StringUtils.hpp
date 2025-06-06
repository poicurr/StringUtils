#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace strutil {

inline char toLower(char c) noexcept {
  return 'A' <= c && c <= 'Z' ? c + 'a' - 'A' : c;
}
inline char toUpper(char c) noexcept {
  return 'a' <= c && c <= 'z' ? c - 'a' + 'A' : c;
}
inline bool isAlpha(char c) noexcept {
  return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}
inline bool isNonZeroDigit(char c) noexcept { return '1' <= c && c <= '9'; }
inline bool isDigit(char c) noexcept { return '0' == c || isNonZeroDigit(c); }
inline bool isCR(char c) noexcept { return c == '\r'; }
inline bool isLF(char c) noexcept { return c == '\n'; }
inline bool isCrlf(char c) noexcept { return isCR(c) || isLF(c); }
inline bool isSpace(char c) noexcept { return c == ' ' || c == '\t'; }
inline bool isLetter(char c) noexcept {
  return isAlpha(c) || isDigit(c) || c == '_';
}
inline bool isCtl(int c) noexcept { return (c >= 0 && c <= 31) || (c == 127); }

inline std::vector<std::string> split(std::string_view s, std::string_view d) {
  std::vector<std::string> ret;
  size_t p1 = 0, p2;
  while ((p2 = s.find(d, p1)) != std::string_view::npos) {
    ret.emplace_back(s.begin() + p1, s.begin() + p2);
    p1 = p2 + d.size();
  }
  ret.emplace_back(s.begin() + p1, s.end());
  return ret;
}

inline bool beginsWith(std::string_view str, std::string_view test) {
  if (str.size() < test.size()) return false;
  return str.compare(0, test.size(), test) == 0;
}

inline bool endsWith(std::string_view str, std::string_view test) {
  if (str.size() < test.size()) return false;
  return str.compare(str.size() - test.size(), test.size(), test) == 0;
}

inline std::string toLower(const std::string& s) {
  auto ret = std::string{};
  ret.reserve(s.size());
  for (const auto c : s) ret += toLower(c);
  return ret;
}

inline std::string toUpper(const std::string& s) {
  auto ret = std::string{};
  ret.reserve(s.size());
  for (const auto c : s) ret += toUpper(c);
  return ret;
}

inline std::string trimLeft(const std::string& s) {
  auto first = s.begin(), last = s.end();
  while (first != last && isSpace(*first)) ++first;
  return {first, last};
}

inline std::string trimRight(const std::string& s) {
  if (s.empty()) return {};
  auto first = s.begin(), last = s.end() - 1;
  while (first <= last && isSpace(*last)) --last;
  return {first, last + 1};
}

inline std::string trim(const std::string& s) { return trimLeft(trimRight(s)); }

inline std::string repeat(char c, size_t n) noexcept {
  return std::string(n, c);
}

inline std::string encode(char c) noexcept {
  static const char a[] = "0123456789ABCDEF";
  return {a[(0xf0 & c) >> 4], a[0x0f & c]};
}

inline std::string encodeURL(const std::string& s) {
  auto ret = std::string{};
  for (char c : s) {
    if (isLetter(c)) {
      ret += c;
      continue;
    }
    ret += "%" + encode(c);
  }
  return ret;
}

inline char decode(const std::string& s) noexcept {
  auto toHexValue = [](char c) -> uint8_t {
    if ('0' <= c && c <= '9') return c - '0';       // '0' ~ '9' ->  0 ~  9
    if ('a' <= c && c <= 'z') return c - 'a' + 10;  // 'a' ~ 'f' -> 10 ~ 15
    if ('A' <= c && c <= 'Z') return c - 'A' + 10;  // 'A' ~ 'F' -> 10 ~ 15
    return 0;
  };
  return static_cast<char>((toHexValue(s[0]) << 4) | toHexValue(s[1]));
}

inline std::string decodeURL(const std::string& s) {
  std::string ret;
  for (size_t i = 0; i < s.size(); ++i) {
    if (s[i] == '%' && i + 2 < s.size()) {
      ret += decode(s.substr(i + 1, 2));
      i += 2;
    } else {
      ret += s[i];
    }
  }
  return ret;
}

inline std::string replace(std::string_view s, std::string_view pattern,
                           std::string_view replacement) {
  if (s.empty()) return {};
  if (pattern.empty()) return {s.begin(), s.end()};

  std::string ret;
  size_t p1 = 0, p2;
  while (true) {
    p2 = s.find(pattern, p1);
    if (p2 == std::string_view::npos) {
      ret += s.substr(p1);
      break;
    }
    ret += s.substr(p1, p2 - p1);
    ret += replacement;
    p1 = p2 + pattern.size();
  }
  return ret;
}

inline std::string join(const std::vector<std::string>& v,
                        const std::string& d) {
  if (v.empty()) return "";
  auto ret = v[0];
  for (size_t i = 1; i < v.size(); ++i) ret += d + v[i];
  return ret;
}

inline bool contains(std::string_view str, std::string_view pattern) noexcept {
  return str.find(pattern) != std::string::npos;
}

inline std::string toUnixPath(const std::string& path) {
  return replace(replace(path, "\\", "/"), "//", "/");
}

}  // namespace strutil
