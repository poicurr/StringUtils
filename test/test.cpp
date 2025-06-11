#define CATCH_CONFIG_MAIN

#include <StringUtils.hpp>
#include <catch2/catch_test_macros.hpp>

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

TEST_CASE("splitLines edge cases", "[splitLines]") {
  auto res = strutil::splitLines("a\n\nb");
  REQUIRE(res.size() == 3);
  REQUIRE(res[0] == "a");
  REQUIRE(res[1] == "");
  REQUIRE(res[2] == "b");

  res = strutil::splitLines("\n");
  REQUIRE(res.size() == 2);
  REQUIRE(res[0] == "");
  REQUIRE(res[1] == "");
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

TEST_CASE("beginsWithIgnoreCase works", "[beginsWithIgnoreCase]") {
  REQUIRE(strutil::beginsWithIgnoreCase("Abcdef", "abc") == true);
  REQUIRE(strutil::beginsWithIgnoreCase("abcdef", "ABC") == true);
  REQUIRE(strutil::beginsWithIgnoreCase("abcdef", "def") == false);
  REQUIRE(strutil::beginsWithIgnoreCase("", "abc") == false);
  REQUIRE(strutil::beginsWithIgnoreCase("abcdef", "") == true);
}

TEST_CASE("endsWithIgnoreCase works", "[endsWithIgnoreCase]") {
  REQUIRE(strutil::endsWithIgnoreCase("abcdef", "DEF") == true);
  REQUIRE(strutil::endsWithIgnoreCase("abcdef", "def") == true);
  REQUIRE(strutil::endsWithIgnoreCase("abcdef", "abc") == false);
  REQUIRE(strutil::endsWithIgnoreCase("", "def") == false);
  REQUIRE(strutil::endsWithIgnoreCase("abcdef", "") == true);
}

TEST_CASE("toLower(std::string) works", "[toLower]") {
  REQUIRE(strutil::toLower("AbCdEfG") == "abcdefg");
}

TEST_CASE("toUpper(std::string) works", "[toUpper]") {
  REQUIRE(strutil::toUpper("abcDEfg") == "ABCDEFG");
}

TEST_CASE("lpad works", "[lpad]") {
  REQUIRE(strutil::lpad("abc", 5) == "  abc");
  REQUIRE(strutil::lpad("abc", 5, '0') == "00abc");
  REQUIRE(strutil::lpad("abc", 2) == "abc");
  REQUIRE(strutil::lpad("", 3, '*') == "***");
}

TEST_CASE("rpad works", "[rpad]") {
  REQUIRE(strutil::rpad("abc", 5) == "abc  ");
  REQUIRE(strutil::rpad("abc", 5, '0') == "abc00");
  REQUIRE(strutil::rpad("abc", 2) == "abc");
  REQUIRE(strutil::rpad("", 3, '*') == "***");
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
  SECTION("Normal Case") {
    REQUIRE(strutil::decode("20") == ' ');
    REQUIRE(strutil::decode("0A") == '\n');
    REQUIRE(strutil::decode("25") == '%');
  }

  SECTION("Nominal Case") {
    REQUIRE_THROWS_AS(strutil::decode(""), std::invalid_argument);
    REQUIRE_THROWS_AS(strutil::decode("A"), std::invalid_argument);
    REQUIRE_THROWS_AS(strutil::decode("123"), std::invalid_argument);
  }
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
  REQUIRE(strutil::toUnixPath(".\\\\path\\to\\file.txt") == "./path/to/file.txt");
}

TEST_CASE("isNumber works", "[isNumber]") {
  REQUIRE(strutil::isNumber("123456") == true);
  REQUIRE(strutil::isNumber("00123") == true);
  REQUIRE(strutil::isNumber("") == false);
  REQUIRE(strutil::isNumber("12a34") == false);
  REQUIRE(strutil::isNumber(" 123") == false);
}

TEST_CASE("splitLines works", "[splitLines]") {
  auto res = strutil::splitLines("a\nb\nc");
  REQUIRE(res.size() == 3);
  REQUIRE(res[0] == "a");
  REQUIRE(res[1] == "b");
  REQUIRE(res[2] == "c");

  res = strutil::splitLines("");
  REQUIRE(res.size() == 1);
  REQUIRE(res[0] == "");
}
