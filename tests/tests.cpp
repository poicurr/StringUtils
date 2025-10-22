#define CATCH_CONFIG_MAIN

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <string>

import strutil;

namespace {

std::string makeUtf8String(const char8_t *literal) {
  return std::string(reinterpret_cast<const char *>(literal));
}

} // namespace

TEST_CASE("toLower works", "[toLower]") {
  auto res = strutil::toLower('a');
  REQUIRE(res == 'a');
  res = strutil::toLower('A');
  REQUIRE(res == 'a');
}

TEST_CASE("toUpper works", "[toUpper]") {
  auto res = strutil::toUpper('A');
  REQUIRE(res == 'A');
  res = strutil::toUpper('a');
  REQUIRE(res == 'A');
}

TEST_CASE("split works", "[split]") {
  SECTION("Normal Case") {
    auto res = strutil::split("aa,bbb,cccc", ",");
    REQUIRE(res.size() == 3);
    REQUIRE(res[0] == "aa");
    REQUIRE(res[1] == "bbb");
    REQUIRE(res[2] == "cccc");
  }

  SECTION("Nominal Case") {
    auto res = strutil::split("", ",");
    REQUIRE(res.size() == 1);
    REQUIRE(res[0] == "");

    auto res2 = strutil::split(",,", ",");
    REQUIRE(res2.size() == 3);
    REQUIRE(res2[0] == "");
    REQUIRE(res2[1] == "");
    REQUIRE(res2[2] == "");
  }
}

TEST_CASE("splitLines works", "[splitLines]") {
  SECTION("Normal Case") {
    auto res = strutil::splitLines("a\n\nb");
    REQUIRE(res.size() == 3);
    REQUIRE(res[0] == "a");
    REQUIRE(res[1] == "");
    REQUIRE(res[2] == "b");

    res = strutil::splitLines("a\r\nb\r\nc");
    REQUIRE(res.size() == 3);
    REQUIRE(res[0] == "a");
    REQUIRE(res[1] == "b");
    REQUIRE(res[2] == "c");
  }

  SECTION("Nominal Case") {
    auto res = strutil::splitLines("\n");
    REQUIRE(res.size() == 2);
    REQUIRE(res[0] == "");
    REQUIRE(res[1] == "");

    res = strutil::splitLines("");
    REQUIRE(res.size() == 1);
    REQUIRE(res[0] == "");
  }
}

TEST_CASE("beginsWith works", "[beginsWith]") {
  REQUIRE(strutil::beginsWith("abcdefg", "abc") == true);
  REQUIRE(strutil::beginsWith("abcdefg", "bc") == false);
  REQUIRE(strutil::beginsWith("", "abc") == false);
  REQUIRE(strutil::beginsWith("abcdefg", "") == true); // !
}

TEST_CASE("endnsWith works", "[endsWith]") {
  REQUIRE(strutil::endsWith("abcdefg", "efg") == true);
  REQUIRE(strutil::endsWith("abcdefg", "ef") == false);
  REQUIRE(strutil::endsWith("", "efg") == false);
  REQUIRE(strutil::endsWith("abcdefg", "") == true); // !
}

TEST_CASE("beginsWith(ignoreCase) works", "[beginsWith][ignoreCase]") {
  REQUIRE(strutil::beginsWith("Abcdef", "abc", true));
  REQUIRE(strutil::beginsWith("abcdef", "ABC", true));
  REQUIRE_FALSE(strutil::beginsWith("abcdef", "def", true));
  REQUIRE_FALSE(strutil::beginsWith("", "abc", true));
  REQUIRE(strutil::beginsWith("abcdef", "", true));
}

TEST_CASE("endsWith(ignoreCase) works", "[endsWith][ignoreCase]") {
  REQUIRE(strutil::endsWith("abcdef", "DEF", true));
  REQUIRE(strutil::endsWith("abcdef", "def", true));
  REQUIRE_FALSE(strutil::endsWith("abcdef", "abc", true));
  REQUIRE_FALSE(strutil::endsWith("", "def", true));
  REQUIRE(strutil::endsWith("abcdef", "", true));
}

TEST_CASE("toLower(std::string) works", "[toLower]") {
  REQUIRE(strutil::toLower("AbCdEfG") == "abcdefg");
}

TEST_CASE("toUpper(std::string) works", "[toUpper]") {
  REQUIRE(strutil::toUpper("abcDEfg") == "ABCDEFG");
}

TEST_CASE("padLeft works", "[padLeft]") {
  REQUIRE(strutil::padLeft("abc", 5) == "  abc");
  REQUIRE(strutil::padLeft("abc", 5, '0') == "00abc");
  REQUIRE(strutil::padLeft("abc", 2) == "abc");
  REQUIRE(strutil::padLeft("", 3, '*') == "***");
}

TEST_CASE("padRight works", "[padRight]") {
  REQUIRE(strutil::padRight("abc", 5) == "abc  ");
  REQUIRE(strutil::padRight("abc", 5, '0') == "abc00");
  REQUIRE(strutil::padRight("abc", 2) == "abc");
  REQUIRE(strutil::padRight("", 3, '*') == "***");
}

TEST_CASE("trimLeft works", "[trimLeft]") {
  REQUIRE(strutil::trimLeft("   abcdefg") == "abcdefg");
  REQUIRE(strutil::trimLeft("  \tabcdefg ") == "abcdefg ");
  REQUIRE(strutil::trimLeft("abcdefg") == "abcdefg");
  REQUIRE(strutil::trimLeft("") == "");
}

TEST_CASE("trimRight works", "[trimRight]") {
  REQUIRE(strutil::trimRight("abcdefg   ") == "abcdefg");
  REQUIRE(strutil::trimRight("  abcdefg \t") == "  abcdefg");
  REQUIRE(strutil::trimRight("abcdefg") == "abcdefg");
  REQUIRE(strutil::trimRight("") == "");
}

TEST_CASE("trim works", "[trim]") {
  REQUIRE(strutil::trim("   abc   ") == "abc");
  REQUIRE(strutil::trim("abc   ") == "abc");
  REQUIRE(strutil::trim("   abc") == "abc");
  REQUIRE(strutil::trim("abc") == "abc");
  REQUIRE(strutil::trim("   ") == "");
  REQUIRE(strutil::trim("") == "");
  REQUIRE(strutil::trim(" \tabc\t ") == "abc");
}

TEST_CASE("encode works", "[url][encode]") {
  REQUIRE(strutil::encode(' ') == "20");
  REQUIRE(strutil::encode('\n') == "0A");
  REQUIRE(strutil::encode('\x7F') == "7F");
  REQUIRE(strutil::encode('%') == "25");
  REQUIRE(strutil::encode('A') == "41");
  REQUIRE(strutil::encode('\x00') == "00");
}

TEST_CASE("encodeURI does not encode reserved characters", "[url][encodeURI]") {
  // unreserved: A-Z a-z 0-9 -_.~
  // reserved: : / ? # [ ] @ ! $ & ' ( ) * + , ; =
  const std::string input =
      makeUtf8String(u8"http://example.com/あいうえお?q=テスト&lang=ja");
  std::string encoded = strutil::encodeURI(input);

  // Reserved chars like ':' '/' '?' '=' '&' should remain
  REQUIRE(encoded.find("http://") == 0);
  REQUIRE(encoded.find("?q=") != std::string::npos);
  REQUIRE(encoded.find("&lang=") != std::string::npos);

  // Non-ASCII parts should be percent-encoded
  REQUIRE(encoded.find("%E3%81%82") != std::string::npos); // あ
  REQUIRE(encoded.find("テスト") == std::string::npos);
}

TEST_CASE("encodeURIComponent / decodeURIComponent round trip",
          "[url][component]") {
  const std::string input = makeUtf8String(u8"こんにちは world&=あいう");
  const auto encoded = strutil::encodeURIComponent(input);
  const auto decoded = strutil::decodeURIComponent(encoded);
  REQUIRE(decoded == input);
  REQUIRE(encoded == "%E3%81%93%E3%82%93%E3%81%AB%E3%81%A1%E3%81%AF%20world%26%"
                     "3D%E3%81%82%E3%81%84%E3%81%86");
}

TEST_CASE("encodePercentAll / decodePercentAll round trip",
          "[url][percentAll]") {
  const std::string raw = makeUtf8String(u8"abc 123 日本語 %\n");
  const auto encoded = strutil::encodePercentAll(raw);
  const auto decoded = strutil::decodePercentAll(encoded);
  REQUIRE(decoded == raw);
  REQUIRE(encoded ==
          "%61%62%63%20%31%32%33%20%E6%97%A5%E6%9C%AC%E8%AA%9E%20%25%0A");
}

TEST_CASE("decode works", "[url][decode]") {
  SECTION("Normal Case") {
    REQUIRE(strutil::decode("20") == ' ');
    REQUIRE(strutil::decode("0A") == '\n');
    REQUIRE(strutil::decode("7F") == '\x7F');
    REQUIRE(strutil::decode("25") == '%');
    REQUIRE(strutil::decode("41") == 'A');
    REQUIRE(strutil::decode("00") == '\0');
  }

  SECTION("Nominal Case") {
    REQUIRE_THROWS_AS(strutil::decode(""), std::invalid_argument);
    REQUIRE_THROWS_AS(strutil::decode("A"), std::invalid_argument);
    REQUIRE_THROWS_AS(strutil::decode("123"), std::invalid_argument);
    REQUIRE_THROWS_AS(strutil::decode("G1"), std::invalid_argument);
  }
}

TEST_CASE("decodeURIComponent works", "[url][decodeURL]") {
  SECTION("Normal Case") {
    REQUIRE(strutil::decodeURIComponent("hello") == "hello");
    REQUIRE(strutil::decodeURIComponent("hello%20world") == "hello world");
    REQUIRE(strutil::decodeURIComponent("abc123_") == "abc123_");
    REQUIRE(strutil::decodeURIComponent("100%25") == "100%");
    REQUIRE(strutil::decodeURIComponent("a%20b%0A") == "a b\n");
  }

  SECTION("Nominal Case") { REQUIRE(strutil::decodeURIComponent("") == ""); }
}

TEST_CASE("encodePercentAll/decodePercentAll with Japanese UTF-8",
          "[url][utf8]") {
  const std::string original = makeUtf8String(u8"こんにちは");
  const std::string encoded = strutil::encodePercentAll(original);
  const std::string decoded = strutil::decodePercentAll(encoded);
  REQUIRE(decoded == original);
}

TEST_CASE("replace works", "[replace]") {
  SECTION("Normal Case") {
    REQUIRE(strutil::replace("abcdefghi", "abc", "ABC") == "ABCdefghi");
    REQUIRE(strutil::replace("abcdefghi", "def", "DEF") == "abcDEFghi");
    REQUIRE(strutil::replace("abcdefghi", "ghi", "GHI") == "abcdefGHI");
  }
  SECTION("Nominal Case") {
    REQUIRE(strutil::replace("", "abc", "ABC") == "");
    REQUIRE(strutil::replace("abcdefghi", "", "ABC") == "abcdefghi");
    REQUIRE(strutil::replace("abcdefghi", "abc", "") == "defghi");
  }
}

TEST_CASE("join works", "[join]") {
  REQUIRE(strutil::join({"abc", "def", "ghi"}, ",") == "abc,def,ghi");
  REQUIRE(strutil::join({"abc"}, ",") == "abc");
  REQUIRE(strutil::join({}, ",") == "");
}

TEST_CASE("toUnixPath works", "[toUnixPath]") {
  REQUIRE(strutil::toUnixPath(".\\path\\to\\file.txt") == "./path/to/file.txt");
  REQUIRE(strutil::toUnixPath("./path/to//file.txt") == "./path/to/file.txt");
  REQUIRE(strutil::toUnixPath(".\\\\path\\to\\file.txt") ==
          "./path/to/file.txt");
}

TEST_CASE("isNumber works", "[isNumber]") {
  REQUIRE(strutil::isNumber("123456") == true);
  REQUIRE(strutil::isNumber("00123") == true);
  REQUIRE(strutil::isNumber("") == false);
  REQUIRE(strutil::isNumber("12a34") == false);
  REQUIRE(strutil::isNumber(" 123") == false);
}

TEST_CASE("strutil::to handles integral inputs", "[to][integral]") {
  REQUIRE(strutil::to<int>("42") == 42);
  REQUIRE(strutil::to<long long>("-9000") == -9000);
  REQUIRE_THROWS_AS(strutil::to<int>("123abc"), std::runtime_error);
  REQUIRE_THROWS_AS(strutil::to<unsigned int>("-1"), std::runtime_error);
}

TEST_CASE("strutil::to handles floating point inputs", "[to][floating]") {
  auto value = strutil::to<double>("3.14159");
  REQUIRE(value == Catch::Approx(3.14159));
  REQUIRE(strutil::to<float>("1e-3") == Catch::Approx(0.001f));
  REQUIRE_THROWS_AS(strutil::to<double>("1.2.3"), std::runtime_error);
}

TEST_CASE("strutil::to handles boolean inputs", "[to][bool]") {
  REQUIRE(strutil::to<bool>("true"));
  REQUIRE_FALSE(strutil::to<bool>("0"));
  REQUIRE_FALSE(strutil::to<bool>(""));
  REQUIRE_THROWS_AS(strutil::to<bool>("maybe"), std::runtime_error);
}

TEST_CASE("strutil::to handles string inputs", "[to][string]") {
  REQUIRE(strutil::to<std::string>(" abc ") == std::string(" abc "));
  REQUIRE(strutil::to<std::string>("") == std::string());
}
