#pragma once

#include <string>
#include <vector>

inline char toLower(char c) { return 'A' <= c && c <= 'Z' ? c + 'a' - 'A' : c; }
inline char toUpper(char c) { return 'a' <= c && c <= 'z' ? c - 'a' - 'A' : c; }
inline bool isAlpha(char c) {
  return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}
inline bool isNonZeroDigit(char c) { return '1' <= c && c <= '9'; }
inline bool isDigit(char c) { return '0' == c || isNonZeroDigit(c); }
inline bool isCR(char c) { return c == '\r'; }
inline bool isLF(char c) { return c == '\n'; }
inline bool isCrlf(char c) { return isCR(c) || isLF(c); }
inline bool isSpace(char c) { return c == ' ' || c == '\t'; }
inline bool isLetter(char c) { return isAlpha(c) || isDigit(c) || c == '_'; }

std::vector<std::string> split(const std::string& s, const std::string& d) {
  if (s.empty()) return {};
  auto ret = std::vector<std::string>{};
  size_t p1 = 0, p2 = std::string::npos;
  while ((p2 = s.find(d, p1)) != std::string::npos) {
    ret.emplace_back(s.begin() + p1, s.begin() + p2);
    p1 = p2 + d.size();
  }
  ret.emplace_back(s.begin() + p1, s.end());
  return ret;
}

bool beginsWith(const std::string& str, const std::string& test) {
  size_t pos = str.find_first_of(test);
  return pos == 0;
}

bool endsWith(const std::string& str, const std::string& test) {
  size_t pos = str.find_last_of(test);
  return pos == str.size() - 1;
}

std::string toLower(const std::string& s) {
  auto ret = std::string{};
  for (const auto c : s) ret += toLower(c);
  return ret;
}

std::string toUpper(const std::string& s) {
  auto ret = std::string{};
  for (const auto c : s) ret += toUpper(c);
  return ret;
}

std::string trimLeft(const std::string& s) {
  auto first = s.begin(), last = s.end();
  while (isSpace(*first) && first != last) {
    ++first;
  }
  return {first, last};
}

std::string trimRight(const std::string& s) {
  auto first = s.begin(), last = first + s.size() - 1;
  while (isSpace(*last) && first != last) {
    --last;
  }
  return {first, last + 1};
}

std::string trim(const std::string& s) { return trimLeft(trimRight(s)); }

std::string repeat(char c, size_t n) { return std::string(n, c); }

std::string encode(char c) {
  static const char a[] = "0123456789ABCDEF";
  auto ret = std::string{};
  ret += a[(0xf0 & c) >> 4];
  ret += a[0x0f & c];
  return ret;
}

std::string encodeURL(const std::string& s) {
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

char decode(const std::string& s) {
  uint8_t c1 = isDigit(s[0]) ? s[0] - '0' : s[0] - 'A' + 10;
  uint8_t c2 = isDigit(s[1]) ? s[1] - '0' : s[1] - 'A' + 10;
  return static_cast<char>((c1 << 4) | c2);
}

std::string decodeURL(const std::string& s) {
  auto ret = std::string{};
  auto p = s.begin(), eol = s.end();
  while (p != eol) {
    if (*p == '%') {
      ++p;
      auto s2 = std::string{p, p + 2};
      ret += decode(s2);
      p += 2;
      continue;
    }
    while (*p != '%' && p != eol) {
      ret += *p;
      ++p;
    }
  }
  return ret;
}

std::string replace(const std::string& s, const std::string& pattern,
                    const std::string& replace) {
  std::string ret;
  size_t p1 = 0, p2 = std::string::npos;
  while (true) {
    p2 = s.find(pattern, p1);
    if (p2 != std::string::npos) {
      ret += std::string{s.begin() + p1, s.begin() + p2};
      ret += replace;
    } else {
      ret += std::string{s.begin() + p1, s.end()};
      break;
    }
    p1 = p2 + pattern.size();
  }
  return ret;
}

std::string join(const std::vector<std::string> v, const std::string& d) {
  if (v.empty()) return "";
  auto ret = v[0];
  for (size_t i = 1; i < v.size(); ++i) {
    ret += d + v[i];
  }
  return ret;
}

bool contains(const std::string& str, const std::string& pattern) {
  return str.find(pattern) != std::string::npos;
}
