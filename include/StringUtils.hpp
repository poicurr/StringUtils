#pragma once

#include <cstdint>
#include <algorithm>
#include <string>
#include <string_view>
#include <stdexcept>
#include <vector>

namespace strutil {

constexpr char toLower(char c) noexcept {
  return 'A' <= c && c <= 'Z' ? c + 'a' - 'A' : c;
}
constexpr char toUpper(char c) noexcept {
  return 'a' <= c && c <= 'z' ? c - 'a' + 'A' : c;
}
constexpr bool isAlpha(char c) noexcept {
  return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}
constexpr bool isNonZeroDigit(char c) noexcept { return '1' <= c && c <= '9'; }
constexpr bool isDigit(char c) noexcept { return '0' <= c && c <= '9'; }
constexpr bool isCR(char c) noexcept { return c == '\r'; }
constexpr bool isLF(char c) noexcept { return c == '\n'; }
constexpr bool isCrlf(char c) noexcept { return isCR(c) || isLF(c); }
constexpr bool isSpace(char c) noexcept { return c == ' ' || c == '\t'; }
constexpr bool isLetter(char c) noexcept {
  return isAlpha(c) || isDigit(c) || c == '_';
}
constexpr bool isCtl(int c) noexcept { return (c >= 0 && c <= 31) || (c == 127); }

inline bool isNumber(std::string_view s) {
  if (s.empty()) return false;
  for (char c : s) if (!isDigit(c)) return false;
  return true;
}

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

inline std::vector<std::string> splitLines(std::string_view s) {
  return split(s, "\n");
}

inline bool beginsWith(std::string_view str, std::string_view prefix) {
  if (str.size() < prefix.size()) return false;
  return str.compare(0, prefix.size(), prefix) == 0;
}

inline bool endsWith(std::string_view str, std::string_view suffix) {
  if (str.size() < suffix.size()) return false;
  return str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

inline bool beginsWithIgnoreCase(std::string_view s, std::string_view prefix) {
  if (s.size() < prefix.size()) return false;
  for (size_t i = 0; i < prefix.size(); ++i)
    if (toLower(s[i]) != toLower(prefix[i])) return false;
  return true;
}

inline bool endsWithIgnoreCase(std::string_view s, std::string_view suffix) {
    if (s.size() < suffix.size()) return false;
    const size_t offset = s.size() - suffix.size();
    for (size_t i = 0; i < suffix.size(); ++i)
        if (toLower(s[offset + i]) != toLower(suffix[i])) return false;
    return true;
}

inline std::string toLower(const std::string& s) {
  std::string ret;
  ret.reserve(s.size());
  std::transform(s.begin(), s.end(), ret.begin(), [](char c) { return toLower(c); });
  return ret;
}

inline std::string toUpper(const std::string& s) {
  std::string ret;
  ret.reserve(s.size());
  std::transform(s.begin(), s.end(), ret.begin(), [](char c) { return toUpper(c); });
  return ret;
}

inline std::string lpad(const std::string& s, size_t len, char pad = ' ') {
  if (s.size() >= len) return s;
  return std::string(len - s.size(), pad) + s;
}

inline std::string rpad(const std::string& s, size_t len, char pad = ' ') {
  if (s.size() >= len) return s;
  return s + std::string(len - s.size(), pad);
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

inline char decode(const std::string& s) {
  if (s.size() != 2) throw std::invalid_argument("Invalid hex string for decode: " + s);
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
