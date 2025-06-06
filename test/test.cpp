#define CATCH_CONFIG_MAIN

#include <StringUtils.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("toLower works", "[toLower]") {
  auto res = strutil::toLower("a");
  REQUIRE(res == "a");
  res = strutil::toLower("A");
  REQUIRE(res == "a");
}

TEST_CASE("toUpper works", "[toUpper]") {
  auto res = strutil::toUpper("A");
  REQUIRE(res == "A");
  res = strutil::toUpper("a");
  REQUIRE(res == "A");
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
  }
}

TEST_CASE("beginsWith works", "[beginsWith]") {
  REQUIRE(strutil::beginsWith("abcdefg", "abc") == true);
  REQUIRE(strutil::beginsWith("abcdefg", "bc") == false);
  REQUIRE(strutil::beginsWith("", "abc") == false);
  REQUIRE(strutil::beginsWith("abcdefg", "") == true);  // !
}

TEST_CASE("endnsWith works", "[endsWith]") {
  REQUIRE(strutil::endsWith("abcdefg", "efg") == true);
  REQUIRE(strutil::endsWith("abcdefg", "ef") == false);
  REQUIRE(strutil::endsWith("", "efg") == false);
  REQUIRE(strutil::endsWith("abcdefg", "") == true);  // !
}

TEST_CASE("toLower(std::string) works", "[toLower]") {
  REQUIRE(strutil::toLower("AbCdEfG") == "abcdefg");
}

TEST_CASE("toUpper(std::string) works", "[toUpper]") {
  REQUIRE(strutil::toUpper("abcDEfg") == "ABCDEFG");
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

TEST_CASE("encode works", "[url][encode]") {
  REQUIRE(strutil::encode(' ') == "20");
  REQUIRE(strutil::encode('\n') == "0A");
  REQUIRE(strutil::encode('%') == "25");
}

TEST_CASE("encodeURL works", "[url][encodeURL]") {
  SECTION("Normal Case") {
    REQUIRE(strutil::encodeURL("hello") == "hello");
    REQUIRE(strutil::encodeURL("hello world") == "hello%20world");
    REQUIRE(strutil::encodeURL("abc123_") == "abc123_");
    REQUIRE(strutil::encodeURL("100%") == "100%25");
    REQUIRE(strutil::encodeURL("a b\n") == "a%20b%0A");
  }

  SECTION("Nominal Case") { REQUIRE(strutil::encodeURL("") == ""); }
}

TEST_CASE("decode works", "[url][decode]") {
  REQUIRE(strutil::decode("20") == ' ');
  REQUIRE(strutil::decode("0A") == '\n');
  REQUIRE(strutil::decode("25") == '%');
}

TEST_CASE("decodeURL works", "[url][decodeURL]") {
  SECTION("Normal Case") {
    REQUIRE(strutil::decodeURL("hello") == "hello");
    REQUIRE(strutil::decodeURL("hello%20world") == "hello world");
    REQUIRE(strutil::decodeURL("abc123_") == "abc123_");
    REQUIRE(strutil::decodeURL("100%25") == "100%");
    REQUIRE(strutil::decodeURL("a%20b%0A") == "a b\n");
  }

  SECTION("Nominal Case") { REQUIRE(strutil::decodeURL("") == ""); }
}

TEST_CASE("encodeURL/decodeURL with Japanese UTF-8", "[url][utf8]") {
  const std::string original = "こんにちは";
  const std::string encoded = strutil::encodeURL(original);
  const std::string decoded = strutil::decodeURL(encoded);
  REQUIRE(decoded == original);
}
